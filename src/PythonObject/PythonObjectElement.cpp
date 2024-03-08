#include <PythonObject.hpp>

#include <Python.hpp>
#include <PythonError.hpp>

#include "PythonForward.ipp"

namespace s3d
{
    using namespace detail;

    PythonObject::PythonObjectElement::PythonObjectElement(const PythonObject &obj, const PythonObject &key)
        : m_objHandler{obj}, m_keyHandler{key} {}

    const PythonObject &PythonObject::PythonObjectElement::operator=(const PythonObject &obj)
    {
        if (PyObject_SetItem(static_cast<PyObject *>(m_objHandler.get()), static_cast<PyObject *>(m_keyHandler.get()), static_cast<PyObject *>(obj.m_handler.get())) < 0)
        {
            ThrowPythonError();
        }
        return obj;
    }

    PythonObject::PythonObjectElement::operator PythonObject() const
    {
        PyObject *itemPtr = PyObject_GetItem(static_cast<PyObject *>(m_objHandler.get()), static_cast<PyObject *>(m_keyHandler.get()));
        if (itemPtr == NULL)
        {
            ThrowPythonError();
        }
        return PythonObjectHandler::FromNewReference(itemPtr);
    }
}