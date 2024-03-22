#pragma once

#include <Siv3D.hpp>

#include "PythonObject.hpp"

namespace s3d
{
    namespace detail
    {
        class PythonOutput
        {
        public:
            /// @brief 任意の型の値に変換して出力を取得する。
            /// @tparam T 変換する型
            /// @param v 値を受け取るための変数
            /// @return このオブジェクト
            /// @note 変換できない場合は何も代入されず、このオブジェクトがfalseに変換されるようになる。
            template <typename T>
            PythonOutput &operator>>(T &v);

            /// @brief 全ての文字を取得する。
            /// @return 全ての文字
            String get();

            /// @brief 残り文字数を取得する。
            /// @return 残り文字数
            size_t getCount() const;

            /// @brief エラーが発生していない場合true, そうでない場合falseを返す。
            operator bool() const;

            /// @brief バッファをクリアする。
            void clear();

            /// @brief Pythonで出力がフラッシュされた時の処理を設定する。
            /// @param onFlush
            void setOnFlush(const std::function<void()> &onFlush);

            /// @brief Pythonで出力がフラッシュされた時の処理を設定する。
            /// @param onFlush
            void setOnFlush(std::function<void()> &&onFlush);

            /// @brief
            /// @param text
            void write(const char *text);

            void flush();

            void setBlocking(bool blocking);

            bool isBlocking() const;

        private:
            mutable std::mutex m_mutex{};

            bool m_blocking : 1 {true};

            bool m_error : 1 {false};

            String m_buf{};

            std::function<void()> m_onFlush{};
        };

        class PythonInput
        {
        public:
            SIV3D_CONCEPT_FORMATTABLE
            PythonInput &operator<<(const Formattable &v);

            std::string read(int64 n = -1);

            std::string readline(int64 limit = -1);

            Array<std::string> readlines(int64 hint = -1);

            void clear();

        private:
            mutable std::mutex m_mutex{};

            std::string m_buf{};
        };

        void ImportSiv3D();
    }

    struct Python
    {
        Python() = delete;

        static detail::PythonOutput StdOut;

        static detail::PythonOutput StdErr;

        static detail::PythonInput StdIn;

        /// @brief 初期化を行う。
        /// @param useStdIO 標準入出力をSiv3Dで使用できるようにする。
        static void Initialize(bool useStdIO = true);

        /// @brief
        static void Finalize();

        /// @brief 初期化済みであるか判定する。
        /// @return 初期化済みであれば true
        static bool IsInitialized() noexcept;

        /// @brief メインモジュールを取得する。
        /// @return メインモジュール
        static const PythonObject &MainModule() noexcept;

        /// @brief グローバル名前空間を取得する。
        /// @return グローバル名前空間
        static const PythonObject &GlobalNS() noexcept;

        /// @brief モジュールをインポートする。
        /// @param moduleName モジュール名
        /// @return モジュール
        static PythonObject Import(StringView moduleName);

        /// @brief モジュールをインポートする。
        /// @param moduleName モジュール名
        /// @return モジュール
        static PythonObject Import(const char *moduleName);

        /// @brief Pythonコードを実行する。
        /// @param code Pythonコード
        /// @param space 名前空間
        static PythonObject Execute(StringView code, const PythonObject &globals = GlobalNS(), const PythonObject &locals = GlobalNS());

        /// @brief Pythonコードを実行する。
        /// @param code Pythonコード
        /// @param space 名前空間
        static PythonObject Execute(const char *code, const PythonObject &globals = GlobalNS(), const PythonObject &locals = GlobalNS());

        /// @brief Pythonファイルを実行する。
        /// @param filePath Pythonファイルのパス
        /// @param space 名前空間
        static PythonObject ExecuteFile(StringView filePath, const PythonObject &globals = GlobalNS(), const PythonObject &locals = GlobalNS());

        /// @brief Pythonファイルを実行する。
        /// @param filePath Pythonファイルのパス
        /// @param space 名前空間
        static PythonObject ExecuteFile(const char *filePath, const PythonObject &globals = GlobalNS(), const PythonObject &locals = GlobalNS());

        /// @brief Pythonの式を実行する。
        /// @param expr Pythonの式
        /// @param space 名前空間
        /// @return 結果
        static PythonObject Eval(StringView expr, const PythonObject &globals = GlobalNS(), const PythonObject &locals = GlobalNS());

        /// @brief Pythonの式を実行する。
        /// @param expr Pythonの式
        /// @param space 名前空間
        /// @return 結果
        static PythonObject Eval(const char *expr, const PythonObject &globals = GlobalNS(), const PythonObject &locals = GlobalNS());

        /// @brief ガベージコレクションを実行する。
        /// @return 検出された到達不可能オブジェクト（破棄不可能なものも含む）の数
        static int64 GarbageCollect();
    };
}

#include "detail/Python.ipp"