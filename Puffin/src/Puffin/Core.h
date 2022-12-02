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

#define BIT(x) (1 << x)

#ifdef PF_ENABLE_ASSERTS
	#define PF_ASSERT(x, ...) { if (!(x)) { PF_ERROR("ASSERTION FAILED: {0}", __VA_ARGS__); __debugbreak(); } }
	#define PF_CORE_ASSERT(x, ...) { if (!(x)) { PF_CORE_ERROR("ASSERTION FAILED: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define PF_ASSERT(x, ...)
	#define PF_CORE_ASSERT(x, ...)
#endif // PF_ENABLE_ASSERTS
