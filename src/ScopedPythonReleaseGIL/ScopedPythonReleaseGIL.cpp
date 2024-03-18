#include <ScopedPythonReleaseGIL.hpp>

#include "PythonForward.ipp"

namespace s3d
{
    ScopedPythonReleaseGIL::ScopedPythonReleaseGIL()
        : m_threadStatePtr(static_cast<void *>(PyEval_SaveThread())) {}

    ScopedPythonReleaseGIL::~ScopedPythonReleaseGIL()
    {
        PyEval_RestoreThread(static_cast<PyThreadState *>(m_threadStatePtr));
    }
}