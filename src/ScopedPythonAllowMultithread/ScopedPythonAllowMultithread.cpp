#include <ScopedPythonAllowMultithread.hpp>

#include "PythonForward.ipp"

namespace s3d
{
    class ScopedPythonAllowMultithread::ScopedPythonAllowMultithreadDetail
    {
    public:
        ScopedPythonAllowMultithreadDetail() : m_threadStatePtr(PyEval_SaveThread()) {}

        ~ScopedPythonAllowMultithreadDetail() { PyEval_RestoreThread(m_threadStatePtr); }

        ScopedPythonAllowMultithreadDetail(const ScopedPythonAllowMultithread &) = delete;

        ScopedPythonAllowMultithreadDetail(ScopedPythonAllowMultithreadDetail &&) = delete;

    private:
        PyThreadState *m_threadStatePtr;
    };

    ScopedPythonAllowMultithread::ScopedPythonAllowMultithread()
        : m_detail{std::make_unique<ScopedPythonAllowMultithreadDetail>()} {}

    ScopedPythonAllowMultithread::~ScopedPythonAllowMultithread() {}
}