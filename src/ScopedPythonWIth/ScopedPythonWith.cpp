#include <ScopedPythonWith.hpp>

#include "PythonForward.ipp"

#include <PythonError.hpp>

ScopedPythonWith::ScopedPythonWith(const PythonObject &obj)
    : PythonObject(obj.getAttr("__enter__")()), m_obj(obj) {}

ScopedPythonWith::ScopedPythonWith(PythonObject &&obj)
    : PythonObject(obj.getAttr("__enter__")()), m_obj(std::move(obj)) {}

ScopedPythonWith::~ScopedPythonWith() { m_obj.getAttr("__exit__")({PythonObject(), PythonObject(), PythonObject()}); }