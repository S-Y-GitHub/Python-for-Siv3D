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
            PyVarObject_HEAD_INIT(NULL, 0)

                .tp_name{"OutputBuffer"},
            .tp_basicsize{sizeof(OutputBuffer)},
            .tp_flags{Py_TPFLAGS_DEFAULT},
            .tp_methods{OutputBuffer_Methods}};

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