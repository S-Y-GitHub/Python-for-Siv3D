#include "PythonOutput.hpp"
#include "PythonOutputBuffer.hpp"

namespace s3d
{
    namespace detail
    {
        PyObject *OutputWrite(PyObject *self, PyObject *s)
        {
            const char *data = PyUnicode_AsUTF8(s);
            if (data == NULL)
            {
                return NULL;
            }
            PyObject *bufferPtr = PyObject_GetAttrString(self, "buffer"); // bufferPtr: New reference
            if (bufferPtr == NULL)
            {
                return NULL;
            }
            if (not PySiv3DOutputBuffer_Write(bufferPtr, data))
            {
                Py_DECREF(bufferPtr);
                return NULL;
            }
            Py_DECREF(bufferPtr);
            Py_RETURN_NONE;
        }

        PyObject *OutputFlush(PyObject *self, PyObject *Py_UNUSED(args))
        {
            PyObject *bufferPtr = PyObject_GetAttrString(self, "buffer"); // bufferPtr: New reference
            if (bufferPtr == NULL)
            {
                return NULL;
            }
            if (not PySiv3DOutputBuffer_Flush(bufferPtr))
            {
                Py_DECREF(bufferPtr);
                return NULL;
            }
            Py_DECREF(bufferPtr);
            Py_RETURN_NONE;
        }

        PyMethodDef Output_Methods[] = {
            {"write", OutputWrite, METH_O, ""},
            {"flush", OutputFlush, METH_NOARGS, ""},
            {NULL}};

#if 12 <= Py_MINOR_VERSION
        PyMemberDef Output_Members[] = {
            {"buffer", Py_T_OBJECT_EX, offsetof(Output, m_bufferPtr), Py_READONLY},
            {NULL}};
#else
        PyMemberDef Output_Members[] = {
            {"buffer", T_OBJECT_EX, offsetof(Output, m_bufferPtr), READONLY},
            {NULL}};
#endif

        PyTypeObject PySiv3DOutput_Type = {
            PyVarObject_HEAD_INIT(NULL, 0)

                .tp_name{"Output"},
            .tp_basicsize{sizeof(Output)},
            .tp_flags{Py_TPFLAGS_DEFAULT},
            .tp_methods{Output_Methods},
            .tp_members{Output_Members}};

        PyObject *PySiv3DOutput_New(PythonOutput *outputPtr)
        {
            PyObject *outputBuffer = PySiv3DOutputBuffer_New(outputPtr);
            if (outputBuffer == NULL)
            {
                return NULL;
            }
            Output *output = PyObject_New(Output, &PySiv3DOutput_Type);
            if (output == NULL)
            {
                Py_DECREF(outputBuffer);
                return NULL;
            }
            output->m_bufferPtr = outputBuffer; //
            return reinterpret_cast<PyObject *>(output);
        }
    }
}