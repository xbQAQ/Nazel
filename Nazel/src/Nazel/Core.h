#pragma once
#ifdef NZ_PLATFORM_WINDOWS
#ifdef NZ_BUILD_DLL
#define NAZEL_API __declspec(dllexport)
#else
#define NAZEL_API __declspec(dllimport)
#endif
#else
#error Nazel only supports Windows!
#endif