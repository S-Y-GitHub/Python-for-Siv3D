#include <ScopedPythonSubthreadGIL.hpp>

#include "PythonForward.ipp"

namespace s3d
{
    class ScopedPythonSubthreadGIL::ScopedPythonSubthreadGILDetail
    {
    public:
        ScopedPythonSubthreadGILDetail() : m_state(PyGILState_Ensure()) {}

        ~ScopedPythonSubthreadGILDetail() { PyGILState_Release(m_state); }

        ScopedPythonSubthreadGILDetail(const ScopedPythonSubthreadGILDetail &) = delete;

        ScopedPythonSubthreadGILDetail(ScopedPythonSubthreadGILDetail &&) = delete;

    private:
        PyGILState_STATE m_state;
    };

    ScopedPythonSubthreadGIL::ScopedPythonSubthreadGIL()
        : m_detail{std::make_unique<ScopedPythonSubthreadGILDetail>()} {}

    ScopedPythonSubthreadGIL::~ScopedPythonSubthreadGIL() {}
}