# Python for Siv3D

## Overview

Siv3DでPythonを使用できる。

## Features

* Pythonコード・ファイルの実行
* Pythonオブジェクトの操作
* Python標準入出力の利用

## Requirement

* Python3.8以上
* Siv3D v0.6.14 (その他のバージョンは未確認)
* macOS (Apple silicomも可・但しX86-64版のPythonが必要)

## Installation

* macOS (Apple silicon)

    Siv3Dは現在のバージョン（v0.6.14）では、Rossetaのみの対応であるため、x86-64版のPythonをインストールする必要があります。

    以下は、x86-64版のHomebrewを使用してPythonをインストールする方法です。

    ```zsh
    # x86-64でzshを起動
    arch -x86_64 zsh
    # x86-64版のHomebrewをインストール
    arch -x86_64 /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install.sh)"
    # x86-64版のPythonをインストール
    arch -x86_64 /usr/local/bin/brew install python3
    ```

    上のコマンドを実行すると、`/usr/local/Cellar/python@(version)`に、最新のPythonがインストールされます。

    このリポジトリの`src`と`include`を適当な場所に配置します。

    Xcodeの`src`（Mainなどがある場所）を右クリックして`Add Files to "(プロジェクト名)"...`からこのリポジトリの`src`内のファイルを追加します。

    Xcodeのプロジェクト設定で、`Build Settings`の`Header Search Paths`に以下のパスを追加します。

    * /usr/local/Cellar/python@(version)/(version)/Frameworks/Python.framework/Headers
    * (このリポジトリのincludeを配置したパス)

    Xcodeのプロジェクト設定で、`Build Settings`の`Library Search Paths`に以下のパスを追加します。

    * /usr/local/Cellar/python@(version)/(version)/Frameworks/Python.framework/Versions/(version)

    Xcodeのプロジェクト設定で、`Build Phases`の`Link Binary With Libraries`に以下のパスを追加します。

    * /usr/local/Cellar/python@(version)/(version)/Frameworks/Python.framework/Versions/(version)/Python

    以上の設定で Python for Siv3D を使用できるようになります。

## Example
```cpp
#include <Siv3D.hpp>

// Python.hppをインクルード
#include <Python.hpp>

void Main()
{
    // インタプリタを初期化
    Python::Initialize();

    // 'Hello, Python.' という式を実行し、結果を取得
    String msg = static_cast<String>(Python::Eval(U"'Hello, Python.'"));

    // 結果を出力
    Print << msg;

    while (System::Update())
    {

    }

	// インタプリタを終了
	Python::Finalize();
}
```