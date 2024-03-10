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
        /// @brief Noneオブジェクトを構築する。
        PythonObject();

        PythonObject(const PythonObject &obj) = default;

        PythonObject(PythonObject &&obj) = default;

        PythonObject(const detail::PythonObjectHandler &handler);

        PythonObject(detail::PythonObjectHandler &&handler);

        /// @brief int型のオブジェクトを構築する。
        /// @param longValue 値
        explicit PythonObject(int64 longValue);

        /// @brief int型のオブジェクトを構築する。
        /// @param longValue 値
        explicit PythonObject(uint64 longValue);

        /// @brief int型のオブジェクトを構築する。
        /// @param longValue 値
        explicit PythonObject(const BigInt &longValue);

        /// @brief int型のオブジェクトを構築する。
        /// @param longValue 値
        SIV3D_CONCEPT_SIGNED_INTEGRAL
        explicit PythonObject(SignedInt longValue);

        /// @brief int型のオブジェクトを構築する。
        /// @param longValue 値
        SIV3D_CONCEPT_UNSIGNED_INTEGRAL
        explicit PythonObject(UnsignedInt longValue);

        /// @brief float型のオブジェクトを構築する。
        /// @param floatValue 値
        explicit PythonObject(double floatValue);

        /// @brief float型のオブジェクトを構築する。
        /// @param floatValue 値
        SIV3D_CONCEPT_FLOATING_POINT
        explicit PythonObject(Float floatValue);

        /// @brief str型のオブジェクトを構築する。
        /// @param strValue 値
        explicit PythonObject(StringView strValue);

        /// @brief str型のオブジェクトを構築する。
        /// @param strValue UTF-8の文字列
        explicit PythonObject(std::string_view strValue);

        /// @brief str型のオブジェクトを構築する。
        /// @param strValue UTF-8の文字列
        explicit PythonObject(const char *strValue);

        /// @brief list型のオブジェクトを構築する。
        /// @param listValue 値
        explicit PythonObject(const Array<PythonObject> &listValue);

        /// @brief list型のオブジェクトを構築する。
        /// @param listValue 要素の初期化リスト
        explicit PythonObject(std::initializer_list<PythonObject> listValue);

        /// @brief dict型のオブジェクトを構築する。
        /// @param dictValue 値
        explicit PythonObject(const HashTable<PythonObject, PythonObject> &dictValue);

        /// @brief 空のリストを返す。
        /// @return 空のリスト
        static PythonObject List();

        /// @brief 空の辞書を返す。
        /// @return 空の辞書
        static PythonObject Dict();

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

        /// @brief 引数なしでこのオブジェクトを呼び出す。
        /// @return 戻り値
        PythonObject operator()() const;

        /// @brief 一つの引数を使用してこのオブジェクトを呼び出す。
        /// @param arg 引数
        /// @return 戻り値
        PythonObject operator()(const PythonObject &arg) const;

        /// @brief 引数リストを使用してこのオブジェクトを呼び出す。
        /// @param args 引数リスト
        /// @return 戻り値
        PythonObject operator()(std::initializer_list<PythonObject> args) const;

        /// @brief キーワード引数リストを使用してこのオブジェクトを呼び出す。
        /// @param kwargs キーワード引数リスト
        /// @return 戻り値
        PythonObject operator()(const HashTable<String, PythonObject> &kwargs) const;

        /// @brief 引数リストとキーワード引数リストを使用してこのオブジェクトを呼び出す。
        /// @param args 引数リスト
        /// @param kwargs キーワード引数リスト
        /// @return 戻り値
        PythonObject operator()(std::initializer_list<PythonObject> args, const HashTable<String, PythonObject> &kwargs) const;

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

        [[nodiscard]] Optional<std::string> getOptUTF8String() const;

        Optional<HashTable<PythonObject, PythonObject>> getOptHashTable() const;

        Optional<Array<PythonObject>> getOptArray() const;

        /// @brief numpyなどのバッファを取得する。
        /// @return バッファの先頭ポインタ
        void *getMemoryView() const;

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