#pragma once
#include <Python.h>
#if PY_MAJOR_VERSION != 3
#error Python3が必要です。
#elif PY_MINOR_VERSION < 8
#error Python3.8以上が必要です
#endif
#include <structmember.h>
