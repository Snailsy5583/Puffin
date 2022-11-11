#pragma once

#ifdef PF_PLATFORM_WINDOWS
#ifdef PF_BUILD_DLL
#define PUFFIN_API __declspec(dllexport)
#else
#define PUFFIN_API __declspec(dllimport)
#endif
#else
#error Windows only.
#endif