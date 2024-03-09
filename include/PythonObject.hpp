#pragma once

#include <Siv3D.hpp>

namespace s3d
{
    namespace detail
    {
        class PythonObjectHandler
        {
        public:
            PythonObjectHandler();

            PythonObjectHandler(const PythonObjectHandler &handler);
            PythonObjectHandler(PythonObjectHandler &&handler);

            PythonObjectHandler &operator=(const PythonObjectHandler &handler);
            PythonObjectHandler &operator=(PythonObjectHandler &&handler);

            ~PythonObjectHandler();

            static PythonObjectHandler FromNewReference(void *ptr);
            static PythonObjectHandler FromBorrowdReference(void *ptr);

            operator void *() const noexcept;

            void *get() const noexcept;

            void *release() noexcept;

            [[nodiscard]] bool operator==(const PythonObjectHandler &handler) const = default;

            [[nodiscard]] bool operator!=(const PythonObjectHandler &handler) const = default;

        private:
            PythonObjectHandler(void *ptr);

            void *m_ptr;
        };
    }

    class PythonObject;

    class PythonObjectIterator
    {
    public:
        PythonObjectIterator();

        PythonObjectIterator(const PythonObject &iterable);

        static PythonObjectIterator EndOf(const PythonObject &iterable);

        [[nodiscard]] PythonObject operator*();

        PythonObjectIterator &operator++();

        [[nodiscard]] bool operator==(const PythonObjectIterator &iterator) const;

        [[nodiscard]] bool operator!=(const PythonObjectIterator &iterator) const;

    private:
        PythonObjectIterator(const detail::PythonObjectHandler &iterableHandler, detail::PythonObjectHandler &&iteratorHandler, detail::PythonObjectHandler &&itemHandler);

        /// @brief Iterable
        detail::PythonObjectHandler m_iterableHandler;

        /// @brief Iterator
        detail::PythonObjectHandler m_iteratorHandler;

        /// @brief current item
        detail::PythonObjectHandler m_itemHandler;
    };

    class PythonObject
    {
        friend std::hash<PythonObject>;

    public:
        PythonObject();

        PythonObject(const PythonObject &obj) = default;

        PythonObject(PythonObject &&obj) = default;

        PythonObject(const detail::PythonObjectHandler &handler);

        PythonObject(detail::PythonObjectHandler &&handler);

        explicit PythonObject(int64 longValue);

        explicit PythonObject(uint64 longValue);

        explicit PythonObject(const BigInt &longValue);

        SIV3D_CONCEPT_SIGNED_INTEGRAL
        explicit PythonObject(SignedInt longValue);

        SIV3D_CONCEPT_UNSIGNED_INTEGRAL
        explicit PythonObject(UnsignedInt longValue);

        explicit PythonObject(double floatValue);

        SIV3D_CONCEPT_FLOATING_POINT
        explicit PythonObject(Float floatValue);

        explicit PythonObject(StringView strValue);

        explicit PythonObject(std::string_view strValue);

        explicit PythonObject(const char *strValue);

        explicit PythonObject(const Array<PythonObject> &listValue);

        explicit PythonObject(std::initializer_list<PythonObject> listValue);

        explicit PythonObject(const HashTable<PythonObject, PythonObject> &dictValue);

        static PythonObject Tuple(const Array<PythonObject> &tupleValue);

        static PythonObject Tuple(std::initializer_list<PythonObject> tupleValue);

        [[nodiscard]] operator bool() const;

        [[nodiscard]] bool operator not() const;

        PythonObject &operator=(const PythonObject &obj);
        PythonObject &operator=(PythonObject &&obj);

        PythonObject operator<(const PythonObject &obj) const;
        PythonObject operator<=(const PythonObject &obj) const;
        PythonObject operator>(const PythonObject &obj) const;
        PythonObject operator>=(const PythonObject &obj) const;
        PythonObject operator==(const PythonObject &obj) const;
        PythonObject operator!=(const PythonObject &obj) const;

        PythonObject operator<<(const PythonObject &obj) const;
        PythonObject operator>>(const PythonObject &obj) const;
        PythonObject operator+(const PythonObject &obj) const;
        PythonObject operator-(const PythonObject &obj) const;
        PythonObject operator*(const PythonObject &obj) const;
        PythonObject operator/(const PythonObject &obj) const;
        PythonObject operator%(const PythonObject &obj) const;
        PythonObject operator&(const PythonObject &obj) const;
        PythonObject operator|(const PythonObject &obj) const;
        PythonObject operator^(const PythonObject &obj) const;

        PythonObject operator<<=(const PythonObject &obj);
        PythonObject operator>>=(const PythonObject &obj);
        PythonObject operator+=(const PythonObject &obj);
        PythonObject operator-=(const PythonObject &obj);
        PythonObject operator*=(const PythonObject &obj);
        PythonObject operator/=(const PythonObject &obj);
        PythonObject operator%=(const PythonObject &obj);
        PythonObject operator&=(const PythonObject &obj);
        PythonObject operator|=(const PythonObject &obj);
        PythonObject operator^=(const PythonObject &obj);

        PythonObject operator+() const;
        PythonObject operator-() const;
        PythonObject operator~() const;

        PythonObject operator()() const;
        PythonObject operator()(const PythonObject &arg) const;
        PythonObject operator()(std::initializer_list<PythonObject> args) const;

        class PythonObjectElement
        {
        public:
            PythonObjectElement(const PythonObject &obj, const PythonObject &key);

            const PythonObject &operator=(const PythonObject &obj);

            operator PythonObject() const;

        private:
            detail::PythonObjectHandler m_objHandler;
            detail::PythonObjectHandler m_keyHandler;
        };

        PythonObjectElement operator[](const PythonObject &index);

        PythonObjectElement operator[](std::initializer_list<PythonObject> indexes);

        PythonObjectIterator begin() const;

        PythonObjectIterator end() const;

        PythonObjectIterator cbegin() const;

        PythonObjectIterator cend() const;

        PythonObject toInt() const;

        PythonObject toFloat() const;

        PythonObject toStr() const;

        PythonObject toList() const;

        PythonObject toDict() const;

        PythonObject toTuple() const;

        [[nodiscard]] int64 getLen() const;

        PythonObject getAttr(const PythonObject &name) const;

        PythonObject getAttr(StringView name) const;

        PythonObject getAttr(const char *name) const;

        PythonObject getType() const;

        [[nodiscard]] bool isNone() const;

        [[nodiscard]] Optional<int64> getOptInt64() const;

        [[nodiscard]] Optional<uint64> getOptUint64() const;

        [[nodiscard]] Optional<BigInt> getOptBigInt() const;

        [[nodiscard]] Optional<double> getOptDouble() const;

        [[nodiscard]] Optional<String> getOptString() const;

        [[nodiscard]] Optional<std::string> getOptNarrowString() const;

        Optional<HashTable<PythonObject, PythonObject>> getOptHashTable() const;

        Optional<Array<PythonObject>> getOptArray() const;

        [[nodiscard]] explicit operator int64() const;

        [[nodiscard]] explicit operator uint64() const;

        [[nodiscard]] explicit operator BigInt() const;

        SIV3D_CONCEPT_SIGNED_INTEGRAL
        [[nodiscard]] explicit operator SignedInt() const;

        SIV3D_CONCEPT_UNSIGNED_INTEGRAL
        [[nodiscard]] explicit operator UnsignedInt() const;

        [[nodiscard]] explicit operator double() const;

        SIV3D_CONCEPT_FLOATING_POINT
        [[nodiscard]] explicit operator Float() const;

        [[nodiscard]] explicit operator String() const;

        [[nodiscard]] explicit operator std::string() const;

        [[nodiscard]] explicit operator Array<PythonObject>() const;

        [[nodiscard]] explicit operator HashTable<PythonObject, PythonObject>() const;

        friend void Formatter(FormatData &formatData, const PythonObject &value);

        operator const detail::PythonObjectHandler &() const noexcept;

        [[nodiscard]] const detail::PythonObjectHandler &getHandler() const noexcept;

        /// @brief abs(a) を計算します。
        /// @param a
        /// @return abs(a)
        static PythonObject Abs(const PythonObject &a);

        /// @brief a // b を計算します。
        /// @param a
        /// @param b
        /// @return a // b
        static PythonObject FloorDiv(const PythonObject &a, const PythonObject &b);

        /// @brief pow(a, b, mod) を計算します。
        /// @param a
        /// @param b
        /// @param mod
        /// @return pow(a, b, mod)
        static PythonObject Pow(const PythonObject &a, const PythonObject &b, const PythonObject &mod = PythonObject{});

        /// @brief divmod(a, b) を計算します。
        /// @param a
        /// @param b
        /// @return divmod(a, b)
        static PythonObject Divmod(const PythonObject &a, const PythonObject &b);

        /// @brief a @ b を計算します。
        /// @param a
        /// @param b
        /// @return a @ b
        static PythonObject MatMul(const PythonObject &a, const PythonObject &b);

        /// @brief a = pow(a, b, mod) を計算します。
        /// @param a
        /// @param b
        /// @param mod
        /// @return a = pow(a, b, mod)
        static PythonObject InPlacePow(PythonObject &a, const PythonObject &b, const PythonObject &mod = PythonObject{});

        /// @brief a @= b を計算します。
        /// @param a
        /// @param b
        /// @return a @= b
        static PythonObject InPlaceMatMul(PythonObject &a, const PythonObject &b);

    private:
        detail::PythonObjectHandler m_handler;
    };

}

template <>
struct std::hash<s3d::PythonObject>
{
    size_t operator()(const s3d::PythonObject &obj) const;
};

template <>
struct SIV3D_HIDDEN fmt::formatter<PythonObject, s3d::char32>
{
    std::u32string tag;

    auto parse(basic_format_parse_context<s3d::char32> &ctx)
    {
        return s3d::detail::GetFormatTag(tag, ctx);
    }

    template <class FormatContext>
    auto format(const PythonObject &value, FormatContext &ctx)
    {
        return format_to(ctx.out(), U"{}", static_cast<String>(value));
    }
};

#include "detail/PythonObject.ipp"