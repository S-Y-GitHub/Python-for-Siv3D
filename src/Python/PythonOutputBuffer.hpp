#pragma once

#include <Python.hpp>

#include "PythonForward.ipp"

namespace s3d
{
    namespace detail
    {
        struct OutputBuffer
        {
            PyObject_HEAD;
            PythonOutput *m_outputPtr;
        };

        /// @brief 文字列を出力します。
        /// @param buffer バッファオブジェクト
        /// @param s 文字列
        /// @return 成功した場合true, そうでなければfalse
        bool PySiv3DOutputBuffer_Write(PyObject *buffer, const char *s);

        /// @brief バッファをフラッシュする。
        /// @param buffer バッファオブジェクト
        /// @return 成功した場合true, そうでなければfalse
        bool PySiv3DOutputBuffer_Flush(PyObject *buffer);

        /// @brief Create new OutputBuffer.
        /// @param outputPtr A pointer of PythonOutput.
        /// @return OutputBuffer: New reference
        PyObject *PySiv3DOutputBuffer_New(PythonOutput *outputPtr);

        extern PyTypeObject PySiv3DOutputBuffer_Type;

#if 9 <= PY_MINOR_VERSION
#define PySiv3DOutputBuffer_CheckExact(op) Py_IS_TYPE(((PyObject *)(((op)))), (&PySiv3DOutputBuffer_Type))
#else
#define PySiv3DOutputBuffer_CheckExact(op) (Py_TYPE(op) == &PySiv3DOutputBuffer_Type)
#endif
    }
}