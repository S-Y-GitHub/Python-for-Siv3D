#pragma once

#include <Siv3D.hpp>

namespace s3d
{
    namespace detail
    {
        struct PythonErrorTraceback
        {
            int64 lineNumber;
            String fileName;
            String funcName;
        };

        void ThrowPythonError();
    }

    class PythonError : public Error
    {
    public:
        PythonError(String &&type, String &&what, Array<detail::PythonErrorTraceback> &&tracebacks);

        StringView type() const noexcept override;

        const Array<detail::PythonErrorTraceback> &getTracebacks() const noexcept;

    private:
        String m_type;
        Array<detail::PythonErrorTraceback> m_tracebacks;
    };

}