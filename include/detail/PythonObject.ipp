#pragma once

#include <PythonObject.hpp>

namespace s3d
{
    SIV3D_CONCEPT_SIGNED_INTEGRAL_
    inline PythonObject::PythonObject(const SignedInt intValue)
        : PythonObject(static_cast<int64>(intValue)) {}

    SIV3D_CONCEPT_UNSIGNED_INTEGRAL_
    inline PythonObject::PythonObject(const UnsignedInt intValue)
        : PythonObject(static_cast<uint64>(intValue)) {}

    SIV3D_CONCEPT_FLOATING_POINT_
    inline PythonObject::PythonObject(const Float floatValue)
        : PythonObject(static_cast<double>(floatValue)) {}

    SIV3D_CONCEPT_SIGNED_INTEGRAL_
    inline PythonObject::operator SignedInt() const { return static_cast<SignedInt>(static_cast<int64>(*this)); }

    SIV3D_CONCEPT_UNSIGNED_INTEGRAL_
    inline PythonObject::operator UnsignedInt() const { return static_cast<UnsignedInt>(static_cast<uint64>(*this)); }

    SIV3D_CONCEPT_FLOATING_POINT_
    inline PythonObject::operator Float() const { return static_cast<Float>(static_cast<double>(*this)); }

}