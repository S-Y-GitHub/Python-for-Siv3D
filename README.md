# Python for Siv3D

## Overview

Siv3DでPythonを使用できます。

## Features

* Pythonコード・ファイルの実行
* Pythonオブジェクトの操作
* Python標準入出力の利用

## Requirement

* Python3.8以上
* Siv3D v0.6.14 (その他のバージョンは未確認)
* macOS (Apple silicomも可・但しX86-64版のPythonが必要)

## Installation

Python C API のヘッダとライブラリがビルド時に必要になります。

* Windows (Visual Studio)

    このリポジトリの`src`と`include`を適当な場所に配置します。
    
    Pythonをインストーラによってインストールした場合、
    Pythonのヘッダは、
    `~/AppData/Local/Programs/Python/Python(version)/include`  
    Pythonのライブラリは、
    `~/AppData/Local/Programs/Python/Python(version)/libs`  
    にあります。

    Visual Studioの `プロジェクト` -> `プロパティ` -> `構成プロパティ` -> `VC++ ディレクトリ` -> `全般` のうち、
    `インクルード ディレクトリ`に以下のパスを追加します。
    * (このリポジトリのincludeを配置したパス)
    * (Pythonのincludeのパス)

    また、`ライブラリ ディレクトリ`に以下のパスを追加します。
    * (Pythonのlibsのパス)

    このライブラリのソースコードはUTF-8を使用して記述されているため、`プロジェクト` -> `プロパティ` -> `構成プロパティ` -> `C/C++` -> `コマンドライン` の`追加のオプション`で `/utf-8` を追加する必要があります。

    `ソリューション エクスプローラ`の`Source Files`にこのリポジトリ内の`src`内のファイルを追加します。

* macOS (Xcode)

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

### Hello, Python.
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

### 基本的な型
```cpp
#include <Siv3D.hpp>

// Python.hppをインクルード
#include <Python.hpp>
// PythonObject.hppをインクルード
#include <PythonObject.hpp>

// int
static void CheckPythonInt()
{
	// 1 + 2 = 3
	PythonObject oneObj(1), twoObj(2);
	Print << U"{} + {} = {}"_fmt(oneObj, twoObj, oneObj + twoObj);

	// int64型に変換
	int64 three = static_cast<int32>(oneObj + twoObj);
	Print << three;

	// 2 ** 128 (Pythonのintは任意長)
	PythonObject expObj(128);
	PythonObject bigNumObj = PythonObject::Pow(twoObj, expObj); // ** 演算子の代わりにPow関数を使用
	Print << U"{} ** {} = {}"_fmt(twoObj, expObj, bigNumObj);

	// BigInt型に変換
	BigInt bigInt = static_cast<BigInt>(bigNumObj);
	Print << U"BigInt: " << bigInt;
}

// float
static void CheckPythonFloat()
{
	// float型のオブジェクトを生成
	PythonObject pi(Math::Pi), e{Math::E};
	Print << U"π * e = {}"_fmt(pi * e);

	// Pythonでは int / int = float
	PythonObject one(1), three(3);
	Print << U"{} / {} = {}"_fmt(one, three, one / three);
	// 整数の除算がしたい場合 (// 演算子の代わりにFloorDiv関数を使用)
	Print << U"{} // {} = {}"_fmt(one, three, PythonObject::FloorDiv(one, three));

	// double型に変換
	double oneThird = static_cast<double>(one / three);
	Print << oneThird;
}

// str
static void CheckPythonStr()
{
	// str型のオブジェクトを生成
	PythonObject helloObj(U"Hello");
	Print << helloObj;

	// String型に変換
	String hello = static_cast<String>(helloObj);
	Print << hello;
}

// dict
static void CheckPythonDict()
{
	// dict型のオブジェクトを生成
	PythonObject dictObj = PythonObject({{PythonObject(U"apple"), PythonObject(120)},
										 {PythonObject(U"orange"), PythonObject(150)},
										 {PythonObject(U"banana"), PythonObject(130)}});
	Print << dictObj;
	// 要素の取得
	Print << dictObj[PythonObject(U"apple")];

	// HashTableに変換
	HashTable<PythonObject, PythonObject> hashTable = static_cast<HashTable<PythonObject, PythonObject>>(dictObj);
	Print << hashTable;
}

// list
static void CheckPythonList()
{
	// list型のオブジェクトを生成
	PythonObject listObj({PythonObject(U"Apple"), PythonObject(2), PythonObject(-0.3)});
	Print << listObj;

	// for
	for (int64 i = 0, n = listObj.getLen(); i < n; ++i)
	{
		Print << listObj[PythonObject(i)];
	}

	// 範囲for
	for (PythonObject elem : listObj)
	{
		Print << elem;
	}

	// Arrayに変換
	Array<PythonObject> array = static_cast<Array<PythonObject>>(listObj);
	Print << array;
}

// tuple
static void CheckPythonTuple()
{
	// tuple型のオブジェクトを生成
	PythonObject tupleObj = PythonObject::Tuple({PythonObject(U"Apple"), PythonObject(2), PythonObject(-0.3)});
	Print << tupleObj;

	// for
	for (int64 i = 0, n = tupleObj.getLen(); i < n; ++i)
	{
		Print << tupleObj[PythonObject(i)];
	}

	// 範囲for
	for (PythonObject elem : tupleObj)
	{
		Print << elem;
	}

	// Arrayに変換
	Array<PythonObject> array = static_cast<Array<PythonObject>>(tupleObj);
	Print << array;
}

void Main()
{
	// インタプリタを初期化
	Python::Initialize();

	// 表示が多いためSceneのサイズを変更
	Scene::Resize(1920, 1080);

	CheckPythonInt();
	CheckPythonFloat();
	CheckPythonStr();
	CheckPythonDict();
	CheckPythonList();
	CheckPythonTuple();

	while (System::Update())
	{
	}

	// インタプリタを終了
	Python::Finalize();
}
```