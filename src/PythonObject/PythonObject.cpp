#include <PythonObject.hpp>

#include <Python.hpp>
#include <PythonError.hpp>

#include "PythonForward.ipp"

namespace s3d
{
    namespace detail
    {
        static PythonObject ToPythonInt(BigInt bigInt)
        {
            PyObject *longPtr = PyLong_FromLong(0);
            if (longPtr == NULL)
            {
                ThrowPythonError();
            }
            PythonObject longObj = PythonObjectHandler::FromNewReference(longPtr);
            if (bigInt.isZero())
            {
                return longObj;
            }
            bool negative = bigInt.sign() < 0;
            if (negative)
            {
                bigInt = -bigInt;
            }
            for (uint64 i = 0; not bigInt.isZero(); i += 64)
            {
                PythonObject temp{(bigInt & BigInt{~static_cast<uint64>(0)}).asUint64()};
                temp <<= PythonObject{i};
                bigInt >>= i;
                longObj |= temp;
            }
            if (negative)
            {
                longObj = -longObj;
            }
            return longObj;
        }

        static BigInt ToBigInt(PythonObject long_)
        {
            BigInt bigInt(0);
            bool negative = long_ < 0;
            if (negative)
            {
                long_ = PythonObject(long_ * -1);
            }
            for (size_t i = 0; long_; i += 64)
            {
                bigInt |= BigInt(static_cast<uint64>(long_ & PythonObject(~static_cast<uint64>(0)))) << i;
                long_ >>= PythonObject(64);
            }
            if (negative)
            {
                bigInt = -bigInt;
            }
            return bigInt;
        }

        static PythonObject ToPythonList(const Array<PythonObject> &array)
        {
            PyObject *listPtr = PyList_New(array.size());
            if (listPtr == NULL)
            {
                ThrowPythonError();
            }
            PythonObjectHandler handler = PythonObjectHandler::FromNewReference(listPtr);
            for (size_t i = 0; i < array.size(); ++i)
            {
                PythonObjectHandler elemHandler = array[i].getHandler();
                Py_INCREF(elemHandler.get());
                PyList_SET_ITEM(listPtr, i, static_cast<PyObject *>(elemHandler.get()));
            }
            return handler;
        }

        static PythonObject ToPythonList(std::initializer_list<PythonObject> initializerList)
        {
            PyObject *listPtr = PyList_New(initializerList.size());
            if (listPtr == NULL)
            {
                ThrowPythonError();
            }
            PythonObjectHandler handler = PythonObjectHandler::FromNewReference(listPtr);
            for (const auto [idx, elem] : Indexed(initializerList))
            {
                PythonObjectHandler elemHandler = elem.getHandler();
                Py_INCREF(elemHandler.get());
                PyList_SET_ITEM(listPtr, idx, static_cast<PyObject *>(elemHandler.get()));
            }
            return handler;
        }

        static Array<PythonObject> ToArray(PythonObject list)
        {
            Array<PythonObject> array;
            for (size_t i = 0, n = list.getLen(); i < n; ++i)
            {
                array << list[PythonObject{i}];
            }
            return array;
        }

        static PythonObject ToPythonDict(const HashTable<PythonObject, PythonObject> &hashTable)
        {
            PyObject *dictPtr = PyDict_New();
            if (dictPtr == NULL)
            {
                ThrowPythonError();
            }
            PythonObject dictObject = PythonObjectHandler::FromNewReference(dictPtr);
            for (const auto &[key, value] : hashTable)
            {
                dictObject[key] = value;
            }
            return dictObject;
        }

        static HashTable<PythonObject, PythonObject> ToHashTable(PythonObject dict)
        {
            HashTable<PythonObject, PythonObject> hashTable;
            PythonObject keys = dict.getAttr("keys")().toTuple();
            for (size_t i = 0, n = keys.getLen(); i < n; ++i)
            {
                PythonObject key = keys[PythonObject{i}];
                PythonObject val = dict[key];
                hashTable[key] = val;
            }
            return hashTable;
        }
    }

    using namespace detail;

    PythonObject::PythonObject()
        : m_handler(PythonObjectHandler::FromBorrowdReference(Py_None)) {}

    PythonObject::PythonObject(const PythonObjectHandler &handler)
        : m_handler(handler) {}

    PythonObject::PythonObject(PythonObjectHandler &&handler)
        : m_handler(std::move(handler)) {}

    PythonObject::PythonObject(const int64 longValue)
    {
        PyObject *longObjectPtr = PyLong_FromLongLong(longValue);
        if (longObjectPtr == NULL)
        {
            ThrowPythonError();
        }
        m_handler = PythonObjectHandler::FromNewReference(longObjectPtr);
    }

    PythonObject::PythonObject(const uint64 longValue)
    {
        PyObject *longObjectPtr = PyLong_FromUnsignedLongLong(longValue);
        if (longObjectPtr == NULL)
        {
            ThrowPythonError();
        }
        m_handler = PythonObjectHandler::FromNewReference(longObjectPtr);
    }

    PythonObject::PythonObject(const BigInt &longValue)
        : m_handler(ToPythonInt(longValue).getHandler()) {}

    PythonObject::PythonObject(const double floatValue)
    {
        PyObject *floatObjectPtr = PyFloat_FromDouble(floatValue);
        if (floatObjectPtr == NULL)
        {
            ThrowPythonError();
        }
        m_handler = PythonObjectHandler::FromNewReference(floatObjectPtr);
    }

    PythonObject::PythonObject(StringView strValue)
        : PythonObject(strValue.toUTF8()) {}

    PythonObject::PythonObject(std::string_view strValue)
    {
        PyObject *strObjectPtr = PyUnicode_DecodeUTF8(strValue.data(), strValue.length(), NULL);
        if (strObjectPtr == NULL)
        {
            ThrowPythonError();
        }
        m_handler = PythonObjectHandler::FromNewReference(strObjectPtr);
    }

    PythonObject::PythonObject(const char *strValue)
    {
        PyObject *strObjectPtr = PyUnicode_DecodeUTF8(strValue, std::strlen(strValue), NULL);
        if (strObjectPtr == NULL)
        {
            ThrowPythonError();
        }
        m_handler = PythonObjectHandler::FromNewReference(strObjectPtr);
    }

    PythonObject::PythonObject(const Array<PythonObject> &listValue)
        : m_handler(ToPythonList(listValue).getHandler()) {}

    PythonObject::PythonObject(std::initializer_list<PythonObject> listValue)
        : m_handler(ToPythonList(listValue).getHandler()) {}

    PythonObject::PythonObject(const HashTable<PythonObject, PythonObject> &dictValue)
        : m_handler(ToPythonDict(dictValue).getHandler()) {}

    PythonObject PythonObject::List()
    {
        PyObject *listPtr = PyList_New(0);
        if (listPtr == NULL)
        {
            ThrowPythonError();
        }
        return PythonObjectHandler::FromNewReference(listPtr);
    }

    PythonObject PythonObject::Dict()
    {
        PyObject *dictPtr = PyDict_New();
        if (dictPtr == NULL)
        {
            ThrowPythonError();
        }
        return PythonObjectHandler::FromNewReference(dictPtr);
    }

    PythonObject PythonObject::Tuple(const Array<PythonObject> &tupleValue)
    {
        PyObject *tuplePtr = PyTuple_New(tupleValue.size());
        if (tuplePtr == NULL)
        {
            ThrowPythonError();
        }
        PythonObjectHandler handler = PythonObjectHandler::FromNewReference(tuplePtr);
        for (const auto [idx, elem] : Indexed(tupleValue))
        {
            PyObject *elemPtr = static_cast<PyObject *>(elem.getHandler().get());
            Py_INCREF(elemPtr);
            PyTuple_SET_ITEM(handler.get(), idx, elemPtr);
        }
        return handler;
    }

    PythonObject PythonObject::Tuple(std::initializer_list<PythonObject> tupleValue)
    {
        PyObject *tuplePtr = PyTuple_New(tupleValue.size());
        if (tuplePtr == NULL)
        {
            ThrowPythonError();
        }
        PythonObjectHandler handler = PythonObjectHandler::FromNewReference(tuplePtr);
        for (const auto [idx, elem] : Indexed(tupleValue))
        {
            PyObject *elemPtr = static_cast<PyObject *>(elem.getHandler().get());
            Py_INCREF(elemPtr);
            PyTuple_SET_ITEM(handler.get(), idx, elemPtr);
        }
        return handler;
    }

    PythonObject &PythonObject::operator=(const PythonObject &obj)
    {
        m_handler = obj.m_handler;
        return *this;
    }

    PythonObject &PythonObject::operator=(PythonObject &&obj)
    {
        m_handler = std::move(obj.m_handler);
        return *this;
    }

    PythonObject PythonObject::getAttr(StringView name) const
    {
        return getAttr(name.toUTF8().c_str());
    }

    PythonObject PythonObject::getAttr(const char *name) const
    {
        PyObject *attrPtr = PyObject_GetAttrString(static_cast<PyObject *>(m_handler.get()), name);
        if (attrPtr == NULL)
        {
            ThrowPythonError();
        }
        return PythonObjectHandler::FromNewReference(attrPtr);
    }

    PythonObject PythonObject::getAttr(const PythonObject &name) const
    {
        PyObject *attrPtr = PyObject_GetAttr(static_cast<PyObject *>(m_handler.get()), static_cast<PyObject *>(name.m_handler.get()));
        if (attrPtr == NULL)
        {
            ThrowPythonError();
        }
        return PythonObjectHandler::FromNewReference(attrPtr);
    }

    void PythonObject::setAttr(StringView name, const PythonObject &val)
    {
        setAttr(name.toUTF8().c_str(), val);
    }

    void PythonObject::setAttr(const char *name, const PythonObject &val)
    {
        if (PyObject_SetAttrString(static_cast<PyObject *>(m_handler.get()),
                                   name,
                                   static_cast<PyObject *>(val.m_handler.get())) != 0)
        {
            ThrowPythonError();
        }
    }

    void PythonObject::setAttr(const PythonObject &name, const PythonObject &val)
    {
        if (PyObject_SetAttr(static_cast<PyObject *>(m_handler.get()),
                             static_cast<PyObject *>(name.m_handler.get()),
                             static_cast<PyObject *>(val.m_handler.get())) != 0)
        {
            ThrowPythonError();
        }
    }

    PythonObject PythonObject::getType() const
    {
        PyObject *typePtr = PyObject_Type(static_cast<PyObject *>(m_handler.get()));
        if (typePtr == NULL)
        {
            ThrowPythonError();
        }
        return PythonObjectHandler::FromNewReference(typePtr);
    }

    PythonObject PythonObject::operator()() const
    {
        PyObject *resultPtr = PyObject_CallObject(static_cast<PyObject *>(m_handler.get()), NULL);
        if (resultPtr == NULL)
        {
            ThrowPythonError();
        }
        return PythonObjectHandler::FromNewReference(resultPtr);
    }

    PythonObject PythonObject::operator()(const PythonObject &arg) const
    {
        PythonObject argsTuple = PythonObject::Tuple({arg});
        PyObject *resultPtr = PyObject_CallObject(static_cast<PyObject *>(m_handler.get()),
                                                  static_cast<PyObject *>(argsTuple.m_handler.get()));
        if (resultPtr == NULL)
        {
            ThrowPythonError();
        }
        return PythonObjectHandler::FromNewReference(resultPtr);
    }

    PythonObject PythonObject::operator()(std::initializer_list<PythonObject> args) const
    {
        const PythonObject argsTuple = Tuple(args);
        PyObject *resultPtr = PyObject_CallObject(static_cast<PyObject *>(m_handler.get()), static_cast<PyObject *>(argsTuple.m_handler.get()));
        if (resultPtr == NULL)
        {
            ThrowPythonError();
        }
        return PythonObjectHandler::FromNewReference(resultPtr);
    }

    PythonObject PythonObject::operator()(const HashTable<String, PythonObject> &kwargs) const
    {
        PythonObject emptyTuple = Tuple({});
        PythonObject kwargsDict = Dict();
        for (const auto &[kw, arg] : kwargs)
        {
            kwargsDict[PythonObject(kw)] = arg;
        }
        PyObject *resultPtr = PyObject_Call(
            static_cast<PyObject *>(m_handler.get()),
            static_cast<PyObject *>(emptyTuple.m_handler.get()),
            static_cast<PyObject *>(kwargsDict.m_handler.get()));
        if (resultPtr == NULL)
        {
            ThrowPythonError();
        }
        return PythonObjectHandler::FromNewReference(resultPtr);
    }

    PythonObject PythonObject::operator()(const HashTable<std::string, PythonObject> &kwargs) const
    {
        PythonObject emptyTuple = Tuple({});
        PythonObject kwargsDict = Dict();
        for (const auto &[kw, arg] : kwargs)
        {
            kwargsDict[PythonObject(kw)] = arg;
        }
        PyObject *resultPtr = PyObject_Call(
            static_cast<PyObject *>(m_handler.get()),
            static_cast<PyObject *>(emptyTuple.m_handler.get()),
            static_cast<PyObject *>(kwargsDict.m_handler.get()));
        if (resultPtr == NULL)
        {
            ThrowPythonError();
        }
        return PythonObjectHandler::FromNewReference(resultPtr);
    }

    PythonObject PythonObject::operator()(std::initializer_list<PythonObject> args, const HashTable<String, PythonObject> &kwargs) const
    {
        PythonObject argsTuple = Tuple(args);
        PythonObject kwargsDict = Dict();
        for (const auto &[kw, arg] : kwargs)
        {
            kwargsDict[PythonObject(kw)] = arg;
        }
        PyObject *resultPtr = PyObject_Call(
            static_cast<PyObject *>(m_handler.get()),
            static_cast<PyObject *>(argsTuple.m_handler.get()),
            static_cast<PyObject *>(kwargsDict.m_handler.get()));
        if (resultPtr == NULL)
        {
            ThrowPythonError();
        }
        return PythonObjectHandler::FromNewReference(resultPtr);
    }

    PythonObject PythonObject::operator()(std::initializer_list<PythonObject> args, const HashTable<std::string, PythonObject> &kwargs) const
    {
        PythonObject argsTuple = Tuple(args);
        PythonObject kwargsDict = Dict();
        for (const auto &[kw, arg] : kwargs)
        {
            kwargsDict[PythonObject(kw)] = arg;
        }
        PyObject *resultPtr = PyObject_Call(
            static_cast<PyObject *>(m_handler.get()),
            static_cast<PyObject *>(argsTuple.m_handler.get()),
            static_cast<PyObject *>(kwargsDict.m_handler.get()));
        if (resultPtr == NULL)
        {
            ThrowPythonError();
        }
        return PythonObjectHandler::FromNewReference(resultPtr);
    }

    PythonObject::PythonObjectElement PythonObject::operator[](const PythonObject &index)
    {
        return {*this, index};
    }

    PythonObject::PythonObjectElement PythonObject::operator[](std::initializer_list<PythonObject> indexes)
    {
        return {*this, Tuple(indexes)};
    }

    PythonObjectIterator PythonObject::begin() const { return {*this}; }

    PythonObjectIterator PythonObject::end() const { return PythonObjectIterator::EndOf(*this); }

    PythonObjectIterator PythonObject::cbegin() const { return begin(); }

    PythonObjectIterator PythonObject::cend() const { return end(); }

    bool PythonObject::isNone() const
    {
#if 10 <= PY_MINOR_VERSION
        return Py_IsNone(m_handler.get());
#else
        return m_handler.get() == Py_None;
#endif
    }

    PythonObject PythonObject::toInt() const
    {
        PyObject *intPtr = PyNumber_Long(static_cast<PyObject *>(m_handler.get()));
        if (intPtr == NULL)
        {
            ThrowPythonError();
        }
        return PythonObjectHandler::FromNewReference(intPtr);
    }

    PythonObject PythonObject::toFloat() const
    {
        PyObject *floatPtr = PyNumber_Float(static_cast<PyObject *>(m_handler.get()));
        if (floatPtr == NULL)
        {
            ThrowPythonError();
        }
        return PythonObjectHandler::FromNewReference(floatPtr);
    }

    PythonObject PythonObject::toStr() const
    {
        PyObject *strPtr = PyObject_Str(static_cast<PyObject *>(m_handler.get()));
        if (strPtr == NULL)
        {
            ThrowPythonError();
        }
        return PythonObjectHandler::FromNewReference(strPtr);
    }

    PythonObject PythonObject::toList() const
    {
        PyObject *listPtr = PySequence_List(static_cast<PyObject *>(m_handler.get()));
        if (listPtr == NULL)
        {
            ThrowPythonError();
        }
        return PythonObjectHandler::FromNewReference(listPtr);
    }

    PythonObject PythonObject::toDict() const
    {
        PyObject *dictPtr = PyObject_CallFunctionObjArgs(reinterpret_cast<PyObject *>(&PyDict_Type), static_cast<PyObject *>(m_handler.get()), NULL);
        if (dictPtr == NULL)
        {
            ThrowPythonError();
        }
        return PythonObjectHandler::FromNewReference(dictPtr);
    }

    PythonObject PythonObject::toTuple() const
    {
        PyObject *tuplePtr = PySequence_Tuple(static_cast<PyObject *>(m_handler.get()));
        if (tuplePtr == NULL)
        {
            ThrowPythonError();
        }
        return PythonObjectHandler::FromNewReference(tuplePtr);
    }

    int64 PythonObject::getLen() const
    {
        int64 val = static_cast<int64>(PyObject_Length(static_cast<PyObject *>(m_handler.get())));
        if (val == -1 && PyErr_Occurred())
        {
            ThrowPythonError();
        }
        return val;
    }

    Optional<int64> PythonObject::getOptInt64() const
    {
        if (not PyLong_Check(static_cast<PyObject *>(m_handler.get())))
        {
            return none;
        }
        int64 val = static_cast<int64>(PyLong_AsLongLong(static_cast<PyObject *>(m_handler.get())));
        if (val == -1 && PyErr_Occurred())
        {
            ThrowPythonError();
        }
        return val;
    }

    Optional<uint64> PythonObject::getOptUint64() const
    {
        if (not PyLong_Check(static_cast<PyObject *>(m_handler.get())))
        {
            return none;
        }
        uint64 val = static_cast<uint64>(PyLong_AsUnsignedLongLong(static_cast<PyObject *>(m_handler.get())));
        if (PyErr_Occurred())
        {
            ThrowPythonError();
        }
        return val;
    }

    Optional<BigInt> PythonObject::getOptBigInt() const
    {
        if (not PyLong_Check(static_cast<PyObject *>(m_handler.get())))
        {
            return none;
        }
        return ToBigInt(*this);
    }

    Optional<double> PythonObject::getOptDouble() const
    {
        if (not PyFloat_Check(static_cast<PyObject *>(m_handler.get())))
        {
            return none;
        }
        double val = PyFloat_AsDouble(static_cast<PyObject *>(m_handler.get()));
        if (val == -1.0 && PyErr_Occurred())
        {
            ThrowPythonError();
        }
        return val;
    }

    Optional<std::string> PythonObject::getOptUTF8String() const
    {
        if (not PyUnicode_Check(static_cast<PyObject *>(m_handler.get())))
        {
            return none;
        }
        const char *val = PyUnicode_AsUTF8(static_cast<PyObject *>(m_handler.get()));
        if (val == NULL)
        {
            ThrowPythonError();
        }
        return val;
    }

    Optional<String> PythonObject::getOptString() const
    {
        if (not PyUnicode_Check(static_cast<PyObject *>(m_handler.get())))
        {
            return none;
        }
        const char *val = PyUnicode_AsUTF8(static_cast<PyObject *>(m_handler.get()));
        if (val == NULL)
        {
            ThrowPythonError();
        }
        return Unicode::FromUTF8(val);
    }

    Optional<HashTable<PythonObject, PythonObject>> PythonObject::getOptHashTable() const
    {
        if (not PyDict_Check(static_cast<PyObject *>(m_handler.get())))
        {
            return none;
        }
        return ToHashTable(*this);
    }

    Optional<Array<PythonObject>> PythonObject::getOptArray() const
    {
        if (not PyList_Check(static_cast<PyObject *>(m_handler.get())))
        {
            return none;
        }
        return ToArray(*this);
    }

    void *PythonObject::getMemoryView() const
    {
        PyObject *memoryViewPtr = PyMemoryView_FromObject(static_cast<PyObject *>(m_handler.get()));
        if (memoryViewPtr == NULL)
        {
            ThrowPythonError();
        }
        Py_buffer *bufferPtr = PyMemoryView_GET_BUFFER(memoryViewPtr);
        void *ptr = bufferPtr->buf;
        Py_DECREF(memoryViewPtr);
        return ptr;
    }

    PythonObject::operator bool() const
    {
        int val = PyObject_IsTrue(static_cast<PyObject *>(m_handler.get()));
        if (val < 0)
        {
            ThrowPythonError();
        }
        return static_cast<bool>(val);
    }

    bool PythonObject::operator not() const
    {
        int val = PyObject_Not(static_cast<PyObject *>(m_handler.get()));
        if (val < 0)
        {
            ThrowPythonError();
        }
        return static_cast<bool>(val);
    }

    PythonObject::operator String() const
    {
        PythonObject str = toStr();
        const char *s = PyUnicode_AsUTF8(static_cast<PyObject *>(str.getHandler().get()));
        if (s == NULL)
        {
            ThrowPythonError();
        }
        return Unicode::FromUTF8(s);
    }

    PythonObject::operator int64() const
    {
        int64 val = PyLong_AsLongLong(static_cast<PyObject *>(m_handler.get()));
        if (val == -1 && PyErr_Occurred())
        {
            ThrowPythonError();
        }
        return val;
    }

    PythonObject::operator uint64() const
    {
        uint64 val = PyLong_AsUnsignedLongLong(static_cast<PyObject *>(m_handler.get()));
        if (val == static_cast<uint64>(-1) && PyErr_Occurred())
        {
            ThrowPythonError();
        }
        return val;
    }

    PythonObject::operator BigInt() const
    {
        return ToBigInt(toInt());
    }

    PythonObject::operator Array<PythonObject>() const
    {
        return ToArray(toList());
    }

    PythonObject::operator HashTable<PythonObject, PythonObject>() const
    {
        return ToHashTable(toDict());
    }

    PythonObject::operator double() const
    {
        PyObject *floatPtr = PyNumber_Float(static_cast<PyObject *>(m_handler.get())); //
        if (floatPtr == NULL)
        {
            ThrowPythonError();
        }
        PythonObjectHandler floatHandler = PythonObjectHandler::FromNewReference(floatPtr);
        double val = PyFloat_AsDouble(static_cast<PyObject *>(floatHandler.get()));
        if (val == -1.0 && PyErr_Occurred())
        {
            ThrowPythonError();
        }
        return val;
    }

    void Formatter(FormatData &formatData, const PythonObject &value)
    {
        formatData.string += static_cast<String>(value);
    }

    const PythonObjectHandler &PythonObject::getHandler() const noexcept { return m_handler; }
}

size_t std::hash<PythonObject>::operator()(const PythonObject &obj) const
{
    return static_cast<size_t>(PyObject_Hash(static_cast<PyObject *>(obj.m_handler.get())));
}
