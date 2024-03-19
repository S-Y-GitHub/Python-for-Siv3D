#include <PythonError.hpp>

#include <PythonObject.hpp>

#include "PythonForward.ipp"

namespace s3d
{
    namespace detail
    {
        void ThrowPythonError()
        {
#if 12 <= PY_MINOR_VERSION
            PyObject *errorPtr = PyErr_GetRaisedException();
            if (errorPtr == NULL)
            {
                throw Error{U"Pythonで補足できない例外が発生しました。"};
            }
            PythonObject errorObj = PythonObjectHandler::FromNewReference(errorPtr);
#else
            PyObject *typePtr, *valuePtr, *tracebackPtr;
            PyErr_Fetch(&typePtr, &valuePtr, &tracebackPtr);
            if (typePtr == NULL)
            {
                throw Error{U"Pythonで補足できない例外が発生しました。"};
            }
            PyErr_NormalizeException(&typePtr, &valuePtr, &tracebackPtr);
            Py_DECREF(typePtr);
            Py_XDECREF(tracebackPtr);
            PythonObject errorObj = PythonObjectHandler::FromNewReference(valuePtr);
#endif
            String typeName = static_cast<String>(errorObj.getType());
            String what = static_cast<String>(errorObj);
            PythonObject tracebackObj = errorObj.getAttr("__traceback__");
            Array<PythonErrorTraceback> tracebacks;
            while (tracebackObj)
            {
                PythonObject fCode = tracebackObj.getAttr("tb_frame").getAttr("f_code");
                tracebacks << PythonErrorTraceback{
                    static_cast<int64>(tracebackObj.getAttr("tb_lineno")),
                    static_cast<String>(fCode.getAttr("co_filename")),
                    static_cast<String>(fCode.getAttr("co_name"))};
                tracebackObj = tracebackObj.getAttr("tb_next");
            }
            PyErr_Clear();
            throw PythonError{std::move(typeName), std::move(what), std::move(tracebacks)};
        }
    }

    using namespace detail;

    PythonError::PythonError(String &&type, String &&what, Array<PythonErrorTraceback> &&tracebacks)
        : Error(std::move(what)), m_type(std::move(type)), m_tracebacks(std::move(tracebacks)) {}

    StringView PythonError::type() const noexcept
    {
        return m_type;
    }

    const Array<PythonErrorTraceback> &PythonError::getTracebacks() const noexcept { return m_tracebacks; }
}