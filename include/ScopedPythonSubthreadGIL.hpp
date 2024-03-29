#pragma once

#include <Siv3D.hpp>

namespace s3d
{
    class ScopedPythonSubthreadGIL : Uncopyable
    {
    public:
        SIV3D_NODISCARD_CXX20
        ScopedPythonSubthreadGIL();

        ~ScopedPythonSubthreadGIL();

    private:
        int m_state;
    };
}