#include "Python.hpp"

#include "PythonInput.hpp"
#include "PythonInputBuffer.hpp"
#include "PythonOutput.hpp"
#include "PythonOutputBuffer.hpp"

#include <Python.h>

namespace s3d
{
    namespace detail
    {
        PyMethodDef Siv3DMethods[] = {
            {NULL}};

        PyModuleDef Siv3DModule = {
            PyModuleDef_HEAD_INIT,
            .m_name{"Siv3D"},
            .m_doc{"Siv3Dの機能をPythonから使用するためのモジュール"},
            .m_size{-1},
            .m_methods{Siv3DMethods},
            .m_slots{NULL},
            .m_traverse{NULL},
            .m_clear{NULL},
            .m_free{NULL}};

        PyMODINIT_FUNC PyInit_Siv3D(void)
        {
            PyObject *siv3dModule = PyModule_Create(&Siv3DModule);
            if (siv3dModule == NULL)
            {
                return NULL;
            }
            {
                PySiv3DOutputBuffer_Type.tp_new = PyType_GenericNew;
                if (PyType_Ready(&PySiv3DOutputBuffer_Type) < 0)
                {
                    return NULL;
                }
                PySiv3DOutput_Type.tp_new = PyType_GenericNew;
                if (PyType_Ready(&PySiv3DOutput_Type) < 0)
                {
                    return NULL;
                }
                {
                    PyObject *stdoutObject = PySiv3DOutput_New(&Python::StdOut); // stdoutObject: New reference
                    if (stdoutObject == NULL)
                    {
                        return NULL;
                    }
#if 10 <= PY_MINOR_VERSION
                    if (PyModule_AddObjectRef(siv3dModule, "stdout", stdoutObject) < 0) // not steal
                    {
                        return NULL;
                    }
#else
                    Py_INCREF(stdoutObject);
                    if (PyModule_AddObject(siv3dModule, "stdout", stdoutObject) < 0)
                    {
                        Py_DECREF(stdoutObject);
                        Py_DECREF(stdoutObject);
                        return NULL;
                    }
#endif
                    if (PySys_SetObject("stdout", stdoutObject) < 0) // Maybe not steal
                    {
                        return NULL;
                    }
                    Py_DECREF(stdoutObject); //
                }
                {
                    PyObject *stderrObject = PySiv3DOutput_New(&Python::StdErr);
                    if (stderrObject == NULL)
                    {
                        return NULL;
                    }
#if 10 <= PY_MINOR_VERSION
                    if (PyModule_AddObjectRef(siv3dModule, "stderr", stderrObject) < 0)
                    {
                        return NULL;
                    }
#else
                    Py_INCREF(stderrObject);
                    if (PyModule_AddObject(siv3dModule, "stderr", stderrObject) < 0)
                    {
                        Py_DECREF(stderrObject);
                        Py_DECREF(stderrObject);
                        return NULL;
                    }
#endif
                    Py_DECREF(stderrObject);
                }
            }
            {
                PySiv3DInputBuffer_Type.tp_new = PyType_GenericNew;
                if (PyType_Ready(&PySiv3DInputBuffer_Type) < 0)
                {
                    return NULL;
                }
                PySiv3DInput_Type.tp_new = PyType_GenericNew;
                if (PyType_Ready(&PySiv3DInput_Type) < 0)
                {
                    return NULL;
                }
                {
                    PyObject *stdinObject = PySiv3DInput_New(&Python::StdIn);
                    if (stdinObject == NULL)
                    {
                        return NULL;
                    }
#if 10 <= PY_MINOR_VERSION
                    if (PyModule_AddObjectRef(siv3dModule, "stdin", stdinObject) < 0)
                    {
                        return NULL;
                    }
#else
                    Py_INCREF(stdinObject);
                    if (PyModule_AddObject(siv3dModule, "stdin", stdinObject) < 0)
                    {
                        Py_DECREF(stdinObject);
                        Py_DECREF(stdinObject);
                        return NULL;
                    }
#endif
                    if (PySys_SetObject("stdin", stdinObject) < 0)
                    {
                        Py_DECREF(stdinObject);
                        return NULL;
                    }
                    Py_DECREF(stdinObject);
                }
            }
            return siv3dModule;
        }

        void ImportSiv3D()
        {
            PyImport_AppendInittab("Siv3D", PyInit_Siv3D);
        }
    }
}