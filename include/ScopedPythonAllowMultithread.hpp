#pragma once

#include <Siv3D.hpp>

namespace s3d
{
    class ScopedPythonAllowMultithread
    {
    public:
        ScopedPythonAllowMultithread();

        ScopedPythonAllowMultithread(ScopedPythonAllowMultithread &&allowMultithread) = default;

        ScopedPythonAllowMultithread(const ScopedPythonAllowMultithread &&) = delete;

        ~ScopedPythonAllowMultithread();

    private:
        struct ScopedPythonAllowMultithreadDetail;

        std::unique_ptr<ScopedPythonAllowMultithreadDetail> m_detail;
    };
}