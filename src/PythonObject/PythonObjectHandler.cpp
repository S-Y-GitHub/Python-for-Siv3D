#include <PythonObject.hpp>

#include "PythonForward.ipp"

namespace s3d
{
    namespace detail
    {
        PythonObjectHandler::PythonObjectHandler()
            : m_ptr{nullptr} {}

        PythonObjectHandler::PythonObjectHandler(const PythonObjectHandler &handler)
            : m_ptr{handler.m_ptr}
        {
            Py_XINCREF(m_ptr); // m_ptr can be null
        }

        PythonObjectHandler::PythonObjectHandler(PythonObjectHandler &&handler)
            : m_ptr{handler.m_ptr}
        {
            handler.m_ptr = nullptr;
        }

        PythonObjectHandler &PythonObjectHandler::operator=(const PythonObjectHandler &handler)
        {
            Py_XDECREF(m_ptr); // m_ptr can be null
            m_ptr = handler.m_ptr;
            Py_XINCREF(m_ptr); // m_ptr can be null
            return *this;
        }

        PythonObjectHandler &PythonObjectHandler::operator=(PythonObjectHandler &&handler)
        {
            m_ptr = handler.m_ptr;
            handler.m_ptr = nullptr;
            return *this;
        }

        PythonObjectHandler::~PythonObjectHandler()
        {
            Py_XDECREF(m_ptr);
        }

        PythonObjectHandler PythonObjectHandler::FromNewReference(void *ptr)
        {
            return {ptr};
        }

        PythonObjectHandler PythonObjectHandler::FromBorrowdReference(void *ptr)
        {
            Py_XINCREF(ptr);
            return {ptr};
        }

        PythonObjectHandler::operator void *() const noexcept { return m_ptr; }

        void *PythonObjectHandler::get() const noexcept { return m_ptr; }

        void *PythonObjectHandler::release() noexcept
        {
            void *temp = m_ptr;
            m_ptr = nullptr;
            return temp;
        }

        PythonObjectHandler::PythonObjectHandler(void *ptr) : m_ptr{ptr} {}
    }
}