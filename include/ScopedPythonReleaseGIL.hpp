#pragma once

#include <Siv3D.hpp>

namespace s3d
{
    class ScopedPythonReleaseGIL : Uncopyable
    {
    public:
        SIV3D_NODISCARD_CXX20
        ScopedPythonReleaseGIL();

        ~ScopedPythonReleaseGIL();

    private:
        void *m_threadStatePtr;
    };
}