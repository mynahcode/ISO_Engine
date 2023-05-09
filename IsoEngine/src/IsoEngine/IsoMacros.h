#pragma once

#ifdef _IE_PLATFORM_WINDOWS
	#ifdef IE_BUILD_DLL
		#define IsoEngine_API __declspec(dllexport)
	#else
		#define IsoEngine_API __declspec(dllimport)
	#endif
#else
	#error IsoEngine currently only supports Windows.
#endif

/* Debug Assertions */
#ifdef IE_ENABLE_ASSERTS
	#define IE_APPLICATION_ASSERT(x, ...) { if(!(x)) {ISOLOGGER_FATAL(("IsoEngine Application Assertion Failed:", x, __VA_ARGS__); __debugbreak(); } } // _debugbreak() is Windows OS only.
	#define IE_ENGINE_ASSERT(x, ...) { if(!(x)) {ISOLOGGER_FATAL(("IsoEngine Core Assertion Failed:", x, __VA_ARGS__); __debugbreak(); } }				// TODO: Implement assertion support for other OS.
#else
	#define IE_APPLICATION_ASSERT(x, ...)
	#define IE_ENGINE_ASSERT(x, ...)
#endif

/* Bit-Field for EventCategory Class*/
#define BIT(x) (1 << x)