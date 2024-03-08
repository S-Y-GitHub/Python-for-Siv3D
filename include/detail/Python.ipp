#pragma once

#include <Python.hpp>

namespace s3d
{
    namespace detail
    {
        template <typename T>
        inline PythonOutput &PythonOutput::operator>>(T &v)
        {
            for (;;)
            {
                std::lock_guard lock{m_mutex};
                size_t l = 0;
                while (l < m_buf.size() && IsSpace(m_buf[l]))
                {
                    ++l;
                }
                if (l < m_buf.size())
                {
                    size_t r = l;
                    do
                    {
                        ++r;
                    } while (r < m_buf.size() && IsSpace(m_buf[r]));
                    String str = m_buf.substr(l, r - l);
                    if (auto optV = ParseOpt<T>(str))
                    {
                        v = *optV;
                        m_buf = m_buf.substr(r);
                    }
                    else
                    {
                        m_error = true;
                    }
                    return *this;
                }
                if (not m_blocking)
                {
                    m_error = true;
                    return *this;
                }
            }
        }

        template <>
        inline PythonOutput &PythonOutput::operator>>(String &v)
        {
            for (;;)
            {
                std::lock_guard lock{m_mutex};
                size_t l = 0;
                while (l < m_buf.size() && IsSpace(m_buf[l]))
                {
                    ++l;
                }
                if (l < m_buf.size())
                {
                    size_t r = l;
                    do
                    {
                        ++r;
                    } while (r < m_buf.size() && IsSpace(m_buf[r]));
                    v = m_buf.substr(l, r - l);
                    m_buf = m_buf.substr(r);
                    return *this;
                }
                if (not m_blocking)
                {
                    m_error = true;
                    return *this;
                }
            }
        }

        SIV3D_CONCEPT_FORMATTABLE_
        inline PythonInput &PythonInput::operator<<(const Formattable &v)
        {
            std::lock_guard lock{m_mutex};
            m_buf += Format(v).toUTF8();
            return *this;
        }
    }
}