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
            PyVarObject_HEAD_INIT(NULL, 0) /// PyObject_VAR_HEAD
            "Output",                      /// tp_name
            sizeof(Output),                /// tp_basicsize
            0,                             /// tp_itemsize
            NULL,                          /// tp_dealloc
            0,                             /// tp_vectorcall_offset
            NULL,                          /// tp_getattr
            NULL,                          /// tp_setattr
            NULL,                          /// tp_as_async
            NULL,                          /// tp_repr
            NULL,                          /// tp_as_number
            NULL,                          /// tp_as_sequence
            NULL,                          /// tp_as_mapping
            NULL,                          /// tp_hash
            NULL,                          /// tp_call
            NULL,                          /// tp_str
            NULL,                          /// tp_getattro
            NULL,                          /// tp_setattro
            NULL,                          /// tp_as_buffer
            Py_TPFLAGS_DEFAULT,            /// tp_flags
            NULL,                          /// tp_doc
            NULL,                          /// tp_traverse
            NULL,                          /// tp_clear
            NULL,                          /// tp_richcompare
            0,                             /// tp_weaklistoffset
            NULL,                          /// tp_iter
            NULL,                          /// tp_iternext
            Output_Methods,                /// tp_methods
            Output_Members};               /// tp_members

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