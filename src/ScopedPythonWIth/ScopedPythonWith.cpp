#include <ScopedPythonWith.hpp>

#include "PythonForward.ipp"

#include <PythonError.hpp>

ScopedPythonWith::ScopedPythonWith(const PythonObject &obj)
    : PythonObject(obj)
{
    if (PyContext_Enter(static_cast<PyObject *>(getHandler().get())) != 0)
    {
        detail::ThrowPythonError();
    }
}

ScopedPythonWith::ScopedPythonWith(PythonObject &&obj)
    : PythonObject(std::move(obj))
{
    if (PyContext_Enter(static_cast<PyObject *>(getHandler().get())) != 0)
    {
        detail::ThrowPythonError();
    }
}

ScopedPythonWith::~ScopedPythonWith()
{
    if (PyContext_Exit(static_cast<PyObject *>(getHandler().get())) != 0)
    {
        detail::ThrowPythonError();
    }
}