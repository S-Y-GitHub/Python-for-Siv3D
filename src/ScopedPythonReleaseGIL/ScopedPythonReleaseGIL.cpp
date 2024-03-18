#include <ScopedPythonReleaseGIL.hpp>

#include "PythonForward.ipp"

namespace s3d
{
    class ScopedPythonReleaseGIL::ScopedPythonReleaseGILDetail
    {
    public:
        ScopedPythonReleaseGILDetail() : m_threadStatePtr(PyEval_SaveThread()) {}

        ~ScopedPythonReleaseGILDetail() { PyEval_RestoreThread(m_threadStatePtr); }

        ScopedPythonReleaseGILDetail(const ScopedPythonReleaseGIL &) = delete;

        ScopedPythonReleaseGILDetail(ScopedPythonReleaseGILDetail &&) = delete;

    private:
        PyThreadState *m_threadStatePtr;
    };

    ScopedPythonReleaseGIL::ScopedPythonReleaseGIL()
        : m_detail(std::make_unique<ScopedPythonReleaseGILDetail>()) {}

    ScopedPythonReleaseGIL::~ScopedPythonReleaseGIL() {}
}