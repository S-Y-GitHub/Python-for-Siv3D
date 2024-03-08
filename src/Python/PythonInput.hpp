#pragma once

#include <Python.hpp>

#include "PythonForward.ipp"

namespace s3d
{
    namespace detail
    {
        struct Input
        {
            PyObject_HEAD;
            PyObject *m_bufferPtr;
        };

        extern PyTypeObject PySiv3DInput_Type;

#if 9 <= PY_MINOR_VERSION
#define PySiv3DInput_CheckExact(op) Py_IS_TYPE(((PyObject *)(((op)))), (&PySiv3DInput_Type))
#else
#define PySiv3DInput_CheckExact(op) (Py_TYPE(op) == &PySiv3DInput_Type)
#endif
        PyObject *PySiv3DInput_New(PythonInput *inputPtr);
    }
}