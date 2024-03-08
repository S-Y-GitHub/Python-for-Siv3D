#pragma once

#include <Python.hpp>

#include "PythonForward.ipp"

namespace s3d
{
    namespace detail
    {
        struct InputBuffer
        {
            PyObject_HEAD;
            PythonInput *m_inputPtr;
        };

        extern PyTypeObject PySiv3DInputBuffer_Type;

#if 9 <= PY_MINOR_VERSION
#define PySiv3DInputBuffer_CheckExact(op) Py_IS_TYPE(((PyObject *)(((op)))), (&PySiv3DInputBuffer_Type))
#else
#define PySiv3DInputBuffer_CheckExact(op) (Py_TYPE(op) == &PySiv3DInputBuffer_Type)
#endif
        Optional<std::string> PySiv3DInputBuffer_Read(PyObject *buffer, int64 n);

        Optional<std::string> PySiv3DInputBuffer_Readline(PyObject *buffer, int64 limit);

        Optional<Array<std::string>> PySiv3DInputBuffer_Readlines(PyObject *buffer, int64 hint);

        PyObject *PySiv3DInputBuffer_New(PythonInput *inputPtr);
    }
}