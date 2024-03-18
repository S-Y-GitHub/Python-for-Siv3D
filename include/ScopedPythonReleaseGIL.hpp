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
        class ScopedPythonReleaseGILDetail;

        std::unique_ptr<ScopedPythonReleaseGILDetail> m_detail;
    };
}