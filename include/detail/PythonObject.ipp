#pragma once

#include <PythonObject.hpp>

namespace s3d
{
    SIV3D_CONCEPT_SIGNED_INTEGRAL_
    inline PythonObject::PythonObject(const SignedInt intValue)
        : PythonObject{static_cast<int64>(intValue)} {}

    SIV3D_CONCEPT_UNSIGNED_INTEGRAL_
    inline PythonObject::PythonObject(const UnsignedInt intValue)
        : PythonObject{static_cast<uint64>(intValue)} {}
}