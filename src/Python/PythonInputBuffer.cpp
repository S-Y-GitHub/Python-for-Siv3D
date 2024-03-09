#include "PythonInputBuffer.hpp"

namespace s3d
{
    namespace detail
    {
        PyObject *InputBufferRead(PyObject *self,
                                  PyObject *const args[],
                                  const Py_ssize_t nargs)
        {
            if (not PySiv3DInputBuffer_CheckExact(self))
            {
                return NULL;
            }
            if (not InRange<Py_ssize_t>(nargs, 0, 1))
            {
                return NULL;
            }
            int64 n = -1;
            if (nargs == 1)
            {
                n = static_cast<int64>(PyLong_AsLongLong(args[0]));
                if (n == -1 && PyErr_Occurred())
                {
                    return NULL;
                }
            }
            std::string s;
            Py_BEGIN_ALLOW_THREADS;
            s = reinterpret_cast<InputBuffer *>(self)->m_inputPtr->read(n);
            Py_END_ALLOW_THREADS;
            PyObject *bytesPtr = PyBytes_FromString(s.c_str());
            if (bytesPtr == NULL)
            {
                return NULL;
            }
            return bytesPtr;
        }

        PyObject *InputBufferReadline(PyObject *self,
                                      PyObject *const args[],
                                      const Py_ssize_t nargs)
        {
            if (not PySiv3DInputBuffer_CheckExact(self))
            {
                return NULL;
            }
            if (not InRange<Py_ssize_t>(nargs, 0, 1))
            {
                return NULL;
            }
            int64 limit = -1;
            if (nargs == 1)
            {
                limit = static_cast<int64>(PyLong_AsSsize_t(args[0]));
                if (limit == -1 && PyErr_Occurred())
                {
                    return NULL;
                }
            }
            std::string line;
            Py_BEGIN_ALLOW_THREADS;
            line = reinterpret_cast<InputBuffer *>(self)->m_inputPtr->readline(limit);
            Py_END_ALLOW_THREADS;
            PyObject *bytesPtr = PyBytes_FromString(line.c_str());
            if (bytesPtr == NULL)
            {
                return NULL;
            }
            return bytesPtr;
        }

        PyMethodDef InputBuffer_Methods[] = {
            {"read", reinterpret_cast<PyCFunction>(InputBufferRead), METH_FASTCALL, ""},
            {"readline", reinterpret_cast<PyCFunction>(InputBufferReadline), METH_FASTCALL, ""},
            {NULL}};

        PyTypeObject PySiv3DInputBuffer_Type = {
            PyVarObject_HEAD_INIT(NULL, 0) /// PyObject_VAR_HEAD
            "InputBuffer",                 /// tp_name
            sizeof(InputBuffer),           /// tp_basicsize
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
            InputBuffer_Methods};          /// tp_methods

        Optional<std::string> PySiv3DInputBuffer_Read(PyObject *buffer, const int64 n)
        {
            if (not PySiv3DInputBuffer_CheckExact(buffer))
            {
                return none;
            }
            std::string s;
            Py_BEGIN_ALLOW_THREADS;
            s = reinterpret_cast<InputBuffer *>(buffer)->m_inputPtr->read(n);
            Py_END_ALLOW_THREADS;
            return s;
        }

        Optional<std::string> PySiv3DInputBuffer_Readline(PyObject *buffer, const int64 limit)
        {
            if (not PySiv3DInputBuffer_CheckExact(buffer))
            {
                return none;
            }
            std::string line;
            Py_BEGIN_ALLOW_THREADS;
            line = reinterpret_cast<InputBuffer *>(buffer)->m_inputPtr->readline(limit);
            Py_END_ALLOW_THREADS;
            return line;
        }

        Optional<Array<std::string>> PySiv3DInputBuffer_Readlines(PyObject *buffer, const int64 hint)
        {
            if (not PySiv3DInputBuffer_CheckExact(buffer))
            {
                return none;
            }
            Array<std::string> lines;
            Py_BEGIN_ALLOW_THREADS;
            lines = reinterpret_cast<InputBuffer *>(buffer)->m_inputPtr->readlines();
            Py_END_ALLOW_THREADS;
            return lines;
        }

        PyObject *PySiv3DInputBuffer_New(PythonInput *inputPtr)
        {
            InputBuffer *inputBuffer = PyObject_New(InputBuffer, &PySiv3DInputBuffer_Type);
            if (inputBuffer == NULL)
            {
                return NULL;
            }
            inputBuffer->m_inputPtr = inputPtr;
            return reinterpret_cast<PyObject *>(inputBuffer);
        }
    }
}