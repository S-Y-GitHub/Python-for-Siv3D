#pragma once

#include <Python.hpp>

#include <Python.h>
#include <structmember.h>

namespace s3d
{
    namespace detail
    {
        struct Output
        {
            PyObject_HEAD;
            PyObject *m_bufferPtr;
        };

        extern PyTypeObject PySiv3DOutput_Type;

#if 9 <= PY_MINOR_VERSION
#define PySiv3DOutput_CheckExact(op) Py_IS_TYPE(((PyObject *)(((op)))), (&PySiv3DOutput_Type))
#else
#define PySiv3DOutput_CheckExact(op) (Py_TYPE(op) == &PySiv3DOutput_Type)
#endif

        PyObject *PySiv3DOutput_New(PythonOutput *outputPtr);
    }
}