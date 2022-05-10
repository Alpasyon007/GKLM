#pragma once

#define DLLEXPORT __declspec( dllexport )

#include <windows.h>
#include <iostream>
#include <map>
#include <vector>
#ifdef GetCurrentTime
#undef GetCurrentTime
#endif