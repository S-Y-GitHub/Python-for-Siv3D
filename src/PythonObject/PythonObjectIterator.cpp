#include <PythonObject.hpp>

#include <Python.hpp>
#include <PythonError.hpp>

#include "PythonForward.ipp"

namespace s3d
{
    using namespace detail;

    PythonObjectIterator::PythonObjectIterator()
        : m_iterableHandler(), m_iteratorHandler(), m_itemHandler() {};

    PythonObjectIterator::PythonObjectIterator(const PythonObject &iterable)
        : m_iterableHandler(iterable.getHandler())
    {
        PyObject *iterPtr = PyObject_GetIter(static_cast<PyObject *>(iterable.getHandler().get()));
        if (iterPtr == NULL)
        {
            ThrowPythonError();
        }
        m_iteratorHandler = PythonObjectHandler::FromNewReference(iterPtr);
        PyObject *firstItemPtr = PyIter_Next(static_cast<PyObject *>(m_iteratorHandler.get()));
        if (firstItemPtr == NULL && PyErr_Occurred())
        {
            ThrowPythonError();
        }
        m_itemHandler = PythonObjectHandler::FromNewReference(firstItemPtr);
    }

    PythonObjectIterator PythonObjectIterator::EndOf(const PythonObject &iterable)
    {
        PyObject *iterPtr = PyObject_GetIter(static_cast<PyObject *>(iterable.getHandler().get()));
        if (iterPtr == NULL)
        {
            ThrowPythonError();
        }
        return {iterable, PythonObjectHandler::FromNewReference(iterPtr), PythonObjectHandler{}};
    }

    PythonObject PythonObjectIterator::operator*() { return m_itemHandler; }

    PythonObjectIterator &PythonObjectIterator::operator++()
    {
        PyObject *itemPtr = PyIter_Next(static_cast<PyObject *>(m_iteratorHandler.get()));
        if (itemPtr == NULL && PyErr_Occurred())
        {
            ThrowPythonError();
        }
        m_itemHandler = PythonObjectHandler::FromNewReference(itemPtr);
        return *this;
    }

    bool PythonObjectIterator::operator==(const PythonObjectIterator &iterator) const
    {
        return m_iterableHandler == iterator.m_iterableHandler && m_itemHandler == iterator.m_itemHandler;
    }

    bool PythonObjectIterator::operator!=(const PythonObjectIterator &iterator) const
    {
        return m_iterableHandler != iterator.m_iterableHandler || m_itemHandler != iterator.m_itemHandler;
    }

    PythonObjectIterator::PythonObjectIterator(const PythonObjectHandler &iterableHandler, PythonObjectHandler &&iteratorHandler, PythonObjectHandler &&itemHandler)
        : m_iterableHandler{iterableHandler}, m_iteratorHandler{std::move(iteratorHandler)}, m_itemHandler{std::move(itemHandler)} {}
}