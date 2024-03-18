#pragma once

#include <Siv3D.hpp>

#include "PythonObject.hpp"

namespace s3d
{
    class ScopedPythonWith : public PythonObject, Uncopyable
    {
    public:
        SIV3D_NODISCARD_CXX20
        ScopedPythonWith(const PythonObject &obj);

        SIV3D_NODISCARD_CXX20
        ScopedPythonWith(PythonObject &&obj);

        ~ScopedPythonWith();

    private:
        PythonObject m_obj;
    };
}