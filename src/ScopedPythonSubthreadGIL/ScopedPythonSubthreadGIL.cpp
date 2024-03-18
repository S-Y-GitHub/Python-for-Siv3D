#include <ScopedPythonSubthreadGIL.hpp>

#include "PythonForward.ipp"

namespace s3d
{
    static_assert(sizeof(PyGILState_STATE) <= sizeof(int));

    ScopedPythonSubthreadGIL::ScopedPythonSubthreadGIL()
        : m_state(static_cast<int>(PyGILState_Ensure())) {}

    ScopedPythonSubthreadGIL::~ScopedPythonSubthreadGIL() { PyGILState_Release(static_cast<PyGILState_STATE>(m_state)); }
}