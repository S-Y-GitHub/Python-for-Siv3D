#include "PythonInput.hpp"
#include "PythonInputBuffer.hpp"

namespace s3d
{
    namespace detail
    {
        static PyObject *InputRead(PyObject *self,
                                   PyObject *const args[],
                                   const Py_ssize_t nargs)
        {
            if (not PySiv3DInput_CheckExact(self))
            {
                return NULL;
            }
            if (not InRange<Py_ssize_t>(nargs, 0, 1))
            {
                return NULL;
            }
            int64 n = -1;
            if (nargs == 1)
            {
                n = static_cast<int64>(PyLong_AsSsize_t(args[0]));
                if (n == -1 && PyErr_Occurred())
                {
                    return NULL;
                }
            }
            PyObject *buffer = PyObject_GetAttrString(self, "buffer"); // buffer: New reference
            if (buffer == NULL)
            {
                return NULL;
            }
            Optional<std::string> s = PySiv3DInputBuffer_Read(buffer, n);
            Py_DECREF(buffer); //
            if (not s)
            {
                return NULL;
            }
            return PyUnicode_DecodeUTF8(s->c_str(), s->size(), NULL);
        }

        static PyObject *InputReadable([[maybe_unused]] PyObject *self,
                                       PyObject *Py_UNUSED(args))
        {
            Py_RETURN_TRUE;
        }

        static PyObject *InputReadline(PyObject *self,
                                       PyObject *const args[],
                                       const Py_ssize_t nargs)
        {
            if (not InRange<Py_ssize_t>(nargs, 0, 1))
            {
                return NULL;
            }
            int64 limit = -1;
            if (nargs == 1)
            {
                limit = static_cast<int64>(PyLong_AsSsize_t(args[0]));
                if (limit == -1 && PyErr_Occurred())
                {
                    return NULL;
                }
            }
            PyObject *buffer = PyObject_GetAttrString(self, "buffer");
            if (buffer == NULL)
            {
                return NULL;
            }
            Optional<std::string> line = PySiv3DInputBuffer_Readline(buffer, limit);
            Py_DECREF(buffer);
            if (not line)
            {
                return NULL;
            }
            return PyUnicode_DecodeUTF8(line->c_str(), line->size(), NULL);
        }

        static PyObject *InputReadlines(PyObject *self,
                                        PyObject *const args[],
                                        const Py_ssize_t nargs)
        {
            if (not InRange<Py_ssize_t>(nargs, 0, 1))
            {
                return NULL;
            }
            int64 hint = -1;
            if (nargs == 1)
            {
                hint = static_cast<int64>(PyLong_AsSsize_t(args[0]));
                if (hint == -1 && PyErr_Occurred())
                {
                    return NULL;
                }
            }
            PyObject *buffer = PyObject_GetAttrString(self, "buffer");
            if (buffer == NULL)
            {
                return NULL;
            }
            Optional<Array<std::string>> lines = PySiv3DInputBuffer_Readlines(buffer, hint);
            Py_DECREF(buffer);
            if (not lines)
            {
                return NULL;
            }
            PyObject *list = PyList_New(lines->size());
            if (list == NULL)
            {
                return NULL;
            }
            for (size_t i = 0; i < lines->size(); ++i)
            {
                PyObject *line = PyUnicode_DecodeUTF8((*lines)[i].c_str(), (*lines)[i].size(), NULL); // line: New reference
                if (line == NULL)
                {
                    Py_DECREF(list);
                    return NULL;
                }
                PyList_SET_ITEM(list, i, line); // line will be stolen.
            }
            return list;
        }

        static PyObject *InputWritable([[maybe_unused]] PyObject *self,
                                       PyObject *Py_UNUSED(args))
        {
            Py_RETURN_FALSE;
        }

        PyMethodDef Input_Methods[] = {
            {"read", reinterpret_cast<PyCFunction>(InputRead), METH_FASTCALL, ""},
            {"readable", reinterpret_cast<PyCFunction>(InputReadable), METH_NOARGS, ""},
            {"readline", reinterpret_cast<PyCFunction>(InputReadline), METH_FASTCALL, ""},
            {"readlines", reinterpret_cast<PyCFunction>(InputReadlines), METH_FASTCALL, ""},
            {"writable", reinterpret_cast<PyCFunction>(InputWritable), METH_NOARGS, ""},
            {NULL}};

#if 12 <= PY_MINOR_VERSION
        PyMemberDef Input_Members[] = {
            {"buffer", Py_T_OBJECT_EX, offsetof(Input, m_bufferPtr), Py_READONLY},
            {NULL}};
#else
        PyMemberDef Input_Members[] = {
            {"buffer", T_OBJECT_EX, offsetof(Input, m_bufferPtr), READONLY},
            {NULL}};
#endif

        PyTypeObject PySiv3DInput_Type = {
            PyVarObject_HEAD_INIT(NULL, 0) /// PyObject_VAR_HEAD
            "Input",                       /// tp_name
            sizeof(Input),                 /// tp_basicsize
            0,                             /// tp_itemsize
            NULL,                          /// tp_dealloc
            0,                             /// tp_vectorcall_offset
            NULL,                          /// tp_getattr
            NULL,                          /// tp_setattr
            NULL,                          /// tp_as_async
            NULL,                          /// tp_repr
            NULL,                          /// tp_as_number
            NULL,                          /// tp_as_sequence
            NULL,                          /// tp_as_mapping
            NULL,                          /// tp_hash
            NULL,                          /// tp_call
            NULL,                          /// tp_str
            NULL,                          /// tp_getattro
            NULL,                          /// tp_setattro
            NULL,                          /// tp_as_buffer
            Py_TPFLAGS_DEFAULT,            /// tp_flags
            NULL,                          /// tp_doc
            NULL,                          /// tp_traverse
            NULL,                          /// tp_clear
            NULL,                          /// tp_richcompare
            0,                             /// tp_weaklistoffset
            NULL,                          /// tp_iter
            NULL,                          /// tp_iternext
            Input_Methods,                 /// tp_methods
            Input_Members};                /// tp_members

        PyObject *PySiv3DInput_New(PythonInput *inputPtr)
        {
            PyObject *inputBuffer = PySiv3DInputBuffer_New(inputPtr);
            if (inputBuffer == NULL)
            {
                return NULL;
            }
            Input *input = PyObject_New(Input, &PySiv3DInput_Type);
            if (input == NULL)
            {
                Py_DECREF(inputBuffer);
                return NULL;
            }
            input->m_bufferPtr = inputBuffer;
            return reinterpret_cast<PyObject *>(input);
        }
    }
}