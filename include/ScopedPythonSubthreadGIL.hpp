#pragma once

#include <Siv3D.hpp>

namespace s3d
{
    class ScopedPythonSubthreadGIL
    {
    public:
        ScopedPythonSubthreadGIL();

        ScopedPythonSubthreadGIL(ScopedPythonSubthreadGIL &&gil) = default;

        ScopedPythonSubthreadGIL(const ScopedPythonSubthreadGIL &) = delete;

        ~ScopedPythonSubthreadGIL();

    private:
        struct ScopedPythonSubthreadGILDetail;

        std::unique_ptr<ScopedPythonSubthreadGILDetail> m_detail;
    };
}