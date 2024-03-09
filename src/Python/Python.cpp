#include <Python.hpp>

#include <PythonError.hpp>
#include <PythonObject.hpp>

#include "PythonForward.ipp"

namespace s3d
{
    static Optional<PythonObject> globalNS;

    static Optional<PythonObject> mainModule;

    namespace detail
    {
        void PythonOutput::clear()
        {
            std::lock_guard lock{m_mutex};
            m_buf.clear();
        }

        String PythonOutput::get()
        {
            std::lock_guard lock{m_mutex};
            String str = m_buf;
            m_buf.clear();
            return str;
        }

        PythonOutput::operator bool() const
        {
            return not m_error;
        }

        size_t PythonOutput::getCount() const
        {
            std::lock_guard lock{m_mutex};
            return m_buf.size();
        }

        void PythonOutput::write(const char *text)
        {
            std::lock_guard lock{m_mutex};
            m_buf += Unicode::Widen(text);
        }

        void PythonOutput::flush()
        {
            if (m_onFlush)
            {
                m_onFlush();
            }
        }

        void PythonOutput::setOnFlush(const std::function<void()> &onFlush)
        {
            m_onFlush = onFlush;
        }

        void PythonOutput::setOnFlush(std::function<void()> &&onFlush)
        {
            m_onFlush = onFlush;
        }

        void PythonInput::clear()
        {
            std::lock_guard lock{m_mutex};
            m_buf.clear();
        }

        std::string PythonInput::read(const int64 n)
        {
            if (n == 0)
            {
                return "";
            }
            std::string buf;
            for (;;)
            {
                std::lock_guard lock{m_mutex};
                for (size_t i = 0; i < m_buf.size(); ++i)
                {
                    if (m_buf[i] == '\0')
                    {
                        return buf;
                    }
                    buf += m_buf[i];
                    if (buf.size() == n)
                    {
                        m_buf = m_buf.substr(i + 1);
                        return buf;
                    }
                }
                m_buf.clear();
            }
        }

        std::string PythonInput::readline(const int64 limit)
        {
            if (limit == 0)
            {
                return "";
            }
            std::string line;
            for (;;)
            {
                std::lock_guard lock{m_mutex};
                for (size_t i = 0; i < m_buf.size(); ++i)
                {
                    if (m_buf[i] == '\0')
                    {
                        return line;
                    }
                    line += m_buf[i];
                    if (m_buf[i] == '\n' ||
                        line.size() == limit)
                    {
                        m_buf = m_buf.substr(i + 1);
                        return line;
                    }
                }
                m_buf.clear();
            }
        }

        Array<std::string> PythonInput::readlines(const int64 hint)
        {
            Array<std::string> lines;
            size_t count = 0;
            while (hint < 0 || 0 < (hint - count))
            {
                std::string line = readline(hint > 0 ? (hint - count) : -1);
                count += line.size();
                lines << line;
            }
            return lines;
        }
    }

    using namespace detail;

    detail::PythonOutput Python::StdOut{};

    detail::PythonOutput Python::StdErr{};

    detail::PythonInput Python::StdIn{};

    void Python::Initialize(const bool useStdIO)
    {
        if (Py_IsInitialized())
        {
            return;
        }
        if (useStdIO)
        {
            detail::ImportSiv3D();
        }
        Py_Initialize(); // Python初期化
        {
            Import("Siv3D");
        }
        mainModule = Import("__main__");            //
        globalNS = mainModule->getAttr("__dict__"); // 大域名前空間を取得
    }

    void Python::Finalize()
    {
        Py_Finalize();
    }

    bool Python::IsInitialized() noexcept
    {
        return Py_IsInitialized();
    }

    const PythonObject &Python::MainModule() noexcept
    {
        return *mainModule;
    }

    const PythonObject &Python::GlobalNS() noexcept
    {
        return *globalNS;
    }

    PythonObject Python::Import(StringView moduleName)
    {
        return Import(moduleName.narrow().c_str());
    }

    PythonObject Python::Import(const char *moduleName)
    {
        return detail::PythonObjectHandler::FromNewReference(PyImport_ImportModule(moduleName));
    }

    PythonObject Python::Execute(StringView code, const PythonObject &globals, const PythonObject &locals)
    {
        return Execute(code.narrow().c_str(), globals, locals);
    }

    PythonObject Python::Execute(const char *code, const PythonObject &globals, const PythonObject &locals)
    {
        PyObject *resultPtr = PyRun_String(code, Py_file_input, static_cast<PyObject *>(globals.getHandler().get()), static_cast<PyObject *>(locals.getHandler().get()));
        if (resultPtr == NULL)
        {
            ThrowPythonError();
        }
        PythonObjectHandler handler = PythonObjectHandler::FromNewReference(resultPtr);
        return handler;
    }

    PythonObject Python::ExecuteFile(StringView filePath, const PythonObject &globals, const PythonObject &locals)
    {
        return ExecuteFile(filePath.narrow().c_str(), globals, locals);
    }

    PythonObject Python::ExecuteFile(const char *filePath, const PythonObject &globals, const PythonObject &locals)
    {
        FILE *fp;
#ifdef __STDC_LIB_EXT1__
        if (std::fopen_s(&fp, filePath, "rb") != 0)
        {
            throw Error{U"ファイル: \"{}\"を開けませんでした。"_fmt(Unicode::Widen(filePath))};
        }
#else
        fp = std::fopen(filePath, "rb");
        if (fp == NULL)
        {
            throw Error{U"ファイル: \"{}\"を開けませんでした。"_fmt(Unicode::Widen(filePath))};
        }
#endif
        PyObject *resultPtr = PyRun_File(fp, filePath, Py_file_input, static_cast<PyObject *>(globals.getHandler().get()), static_cast<PyObject *>(locals.getHandler().get()));
        if (resultPtr == NULL)
        {
            ThrowPythonError();
        }
        PythonObjectHandler handler = PythonObjectHandler::FromNewReference(resultPtr);
        return handler;
    }

    PythonObject Python::Eval(StringView expr, const PythonObject &globals, const PythonObject &locals)
    {
        return Eval(expr.narrow().c_str(), globals, locals);
    }

    PythonObject Python::Eval(const char *expr, const PythonObject &globals, const PythonObject &locals)
    {
        PyObject *resultPtr = PyRun_String(expr, Py_eval_input, static_cast<PyObject *>(globals.getHandler().get()), static_cast<PyObject *>(locals.getHandler().get()));
        if (resultPtr == NULL)
        {
            ThrowPythonError();
        }
        PythonObjectHandler handler = PythonObjectHandler::FromNewReference(resultPtr);
        return handler;
    }
}
