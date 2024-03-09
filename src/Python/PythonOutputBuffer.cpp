#include "PythonOutputBuffer.hpp"

namespace s3d
{
    namespace detail
    {
        PyObject *OutputBufferWrite(PyObject *self, PyObject *s)
        {
            if (not PySiv3DOutputBuffer_CheckExact(self))
            {
                return NULL;
            }
            const char *bytes = PyBytes_AsString(s);
            if (bytes == NULL)
            {
                return NULL;
            }
            Py_BEGIN_ALLOW_THREADS;
            reinterpret_cast<OutputBuffer *>(self)->m_outputPtr->write(bytes);
            Py_END_ALLOW_THREADS;
            Py_RETURN_NONE;
        }

        PyObject *OutputBufferFlush(PyObject *self, PyObject *Py_UNUSED(args))
        {
            if (not PySiv3DOutputBuffer_CheckExact(self))
            {
                return NULL;
            }
            Py_BEGIN_ALLOW_THREADS;
            reinterpret_cast<OutputBuffer *>(self)->m_outputPtr->flush();
            Py_END_ALLOW_THREADS;
            Py_RETURN_NONE;
        }

        bool PySiv3DOutputBuffer_Write(PyObject *buffer, const char *s)
        {
            if (not PySiv3DOutputBuffer_CheckExact(buffer))
            {
                return false;
            }
            Py_BEGIN_ALLOW_THREADS;
            reinterpret_cast<OutputBuffer *>(buffer)->m_outputPtr->write(s);
            Py_END_ALLOW_THREADS;
            return true;
        }

        bool PySiv3DOutputBuffer_Flush(PyObject *buffer)
        {
            if (not PySiv3DOutputBuffer_CheckExact(buffer))
            {
                return false;
            }
            Py_BEGIN_ALLOW_THREADS;
            reinterpret_cast<OutputBuffer *>(buffer)->m_outputPtr->flush();
            Py_END_ALLOW_THREADS;
            return true;
        }

        PyMethodDef OutputBuffer_Methods[] = {
            {"write", OutputBufferWrite, METH_O, ""},
            {"flush", OutputBufferFlush, METH_NOARGS, ""},
            {NULL}};

        PyTypeObject PySiv3DOutputBuffer_Type = {
            PyVarObject_HEAD_INIT(NULL, 0) /// PyObject_VAR_HEAD
            "OutputBuffer",                /// tp_name
            sizeof(OutputBuffer),          /// tp_basicsize
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
            OutputBuffer_Methods};         /// tp_methods

        PyObject *PySiv3DOutputBuffer_New(PythonOutput *outputPtr)
        {
            OutputBuffer *outputBuffer = PyObject_New(OutputBuffer, &PySiv3DOutputBuffer_Type); // outputBuffer: New reference
            if (outputBuffer == NULL)
            {
                return NULL;
            }
            outputBuffer->m_outputPtr = outputPtr;
            return reinterpret_cast<PyObject *>(outputBuffer);
        }
    }
}