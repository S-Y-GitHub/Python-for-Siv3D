#include <PythonObject.hpp>

#include <Python.hpp>
#include <PythonError.hpp>

#include "PythonForward.ipp"

namespace s3d
{
    using namespace detail;

#define OPERATOR(op, func)                                                                                                  \
    PythonObject PythonObject::operator op(const PythonObject & obj) const                                                  \
    {                                                                                                                       \
        PyObject *resultPtr = func(static_cast<PyObject *>(m_handler.get()), static_cast<PyObject *>(obj.m_handler.get())); \
        if (resultPtr == NULL)                                                                                              \
        {                                                                                                                   \
            ThrowPythonError();                                                                                             \
        }                                                                                                                   \
        return PythonObjectHandler::FromNewReference(resultPtr);                                                            \
    }

    OPERATOR(<<, PyNumber_Lshift)
    OPERATOR(>>, PyNumber_Rshift)
    OPERATOR(+, PyNumber_Add)
    OPERATOR(-, PyNumber_Subtract)
    OPERATOR(*, PyNumber_Multiply)
    OPERATOR(/, PyNumber_TrueDivide)
    OPERATOR(%, PyNumber_Remainder)
    OPERATOR(&, PyNumber_And)
    OPERATOR(|, PyNumber_Or)
    OPERATOR(^, PyNumber_Xor)

#define INPLACE_OPERATOR(op, func)                                                                                          \
    PythonObject PythonObject::operator op(const PythonObject & obj)                                                        \
    {                                                                                                                       \
        PyObject *resultPtr = func(static_cast<PyObject *>(m_handler.get()), static_cast<PyObject *>(obj.m_handler.get())); \
        if (resultPtr == NULL)                                                                                              \
        {                                                                                                                   \
            ThrowPythonError();                                                                                             \
        }                                                                                                                   \
        return *this = PythonObjectHandler::FromNewReference(resultPtr);                                                    \
    }

    INPLACE_OPERATOR(<<=, PyNumber_InPlaceLshift)
    INPLACE_OPERATOR(>>=, PyNumber_InPlaceRshift)
    INPLACE_OPERATOR(+=, PyNumber_InPlaceAdd)
    INPLACE_OPERATOR(-=, PyNumber_InPlaceSubtract)
    INPLACE_OPERATOR(*=, PyNumber_InPlaceMultiply)
    INPLACE_OPERATOR(/=, PyNumber_InPlaceTrueDivide)
    INPLACE_OPERATOR(%=, PyNumber_InPlaceRemainder)
    INPLACE_OPERATOR(&=, PyNumber_InPlaceAnd)
    INPLACE_OPERATOR(|=, PyNumber_InPlaceOr)
    INPLACE_OPERATOR(^=, PyNumber_InPlaceXor)

#define UNARY_OPERATOR(op, func)                                              \
    PythonObject PythonObject::operator op() const                            \
    {                                                                         \
        PyObject *resultPtr = func(static_cast<PyObject *>(m_handler.get())); \
        if (resultPtr == NULL)                                                \
        {                                                                     \
            ThrowPythonError();                                               \
        }                                                                     \
        return PythonObjectHandler::FromNewReference(resultPtr);              \
    }

    UNARY_OPERATOR(+, PyNumber_Positive)
    UNARY_OPERATOR(-, PyNumber_Negative)
    UNARY_OPERATOR(~, PyNumber_Invert)

#define CMP_OPERATOR(op, opid)                                                                                                                    \
    PythonObject PythonObject::operator op(const PythonObject & obj) const                                                                        \
    {                                                                                                                                             \
        PyObject *resultPtr = PyObject_RichCompare(static_cast<PyObject *>(m_handler.get()), static_cast<PyObject *>(obj.m_handler.get()), opid); \
        if (resultPtr == NULL)                                                                                                                    \
        {                                                                                                                                         \
            ThrowPythonError();                                                                                                                   \
        }                                                                                                                                         \
        return PythonObjectHandler::FromNewReference(resultPtr);                                                                                  \
    }
    CMP_OPERATOR(<, Py_LT)
    CMP_OPERATOR(<=, Py_LE)
    CMP_OPERATOR(==, Py_EQ)
    CMP_OPERATOR(!=, Py_NE)
    CMP_OPERATOR(>, Py_GT)
    CMP_OPERATOR(>=, Py_GE)

    PythonObject PythonObject::Abs(const PythonObject &a)
    {
        PyObject *resultPtr = PyNumber_Absolute(static_cast<PyObject *>(a.getHandler().get()));
        if (resultPtr == NULL)
        {
            ThrowPythonError();
        }
        return PythonObjectHandler::FromNewReference(resultPtr);
    }

    PythonObject PythonObject::FloorDiv(const PythonObject &a, const PythonObject &b)
    {
        PyObject *resultPtr = PyNumber_FloorDivide(static_cast<PyObject *>(a.getHandler().get()), static_cast<PyObject *>(b.getHandler().get()));
        if (resultPtr == NULL)
        {
            ThrowPythonError();
        }
        return PythonObjectHandler::FromNewReference(resultPtr);
    }

    PythonObject PythonObject::Pow(const PythonObject &a, const PythonObject &b, const PythonObject &mod)
    {
        PyObject *resultPtr = PyNumber_Power(static_cast<PyObject *>(a.getHandler().get()), static_cast<PyObject *>(b.getHandler().get()), static_cast<PyObject *>(mod.getHandler().get()));
        if (resultPtr == NULL)
        {
            ThrowPythonError();
        }
        return PythonObjectHandler::FromNewReference(resultPtr);
    }

    PythonObject PythonObject::Divmod(const PythonObject &a, const PythonObject &b)
    {
        PyObject *resultPtr = PyNumber_Divmod(static_cast<PyObject *>(a.getHandler().get()), static_cast<PyObject *>(b.getHandler().get()));
        if (resultPtr == NULL)
        {
            ThrowPythonError();
        }
        return PythonObjectHandler::FromNewReference(resultPtr);
    }

    PythonObject PythonObject::MatMul(const PythonObject &a, const PythonObject &b)
    {
        PyObject *resultPtr = PyNumber_MatrixMultiply(static_cast<PyObject *>(a.getHandler().get()), static_cast<PyObject *>(b.getHandler().get()));
        if (resultPtr == NULL)
        {
            ThrowPythonError();
        }
        return PythonObjectHandler::FromNewReference(resultPtr);
    }

    PythonObject PythonObject::InPlacePow(PythonObject &a, const PythonObject &b, const PythonObject &mod)
    {
        PyObject *resultPtr = PyNumber_InPlacePower(static_cast<PyObject *>(a.getHandler().get()), static_cast<PyObject *>(b.getHandler().get()), static_cast<PyObject *>(mod.getHandler().get()));
        if (resultPtr == NULL)
        {
            ThrowPythonError();
        }
        return PythonObjectHandler::FromNewReference(resultPtr);
    }

    PythonObject PythonObject::InPlaceMatMul(PythonObject &a, const PythonObject &b)
    {
        PyObject *resultPtr = PyNumber_MatrixMultiply(static_cast<PyObject *>(a.getHandler().get()), static_cast<PyObject *>(b.getHandler().get()));
        if (resultPtr == NULL)
        {
            ThrowPythonError();
        }
        return (a = PythonObjectHandler::FromNewReference(resultPtr));
    }
}