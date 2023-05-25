#pragma once

#include <memory>

#ifdef _IE_PLATFORM_WINDOWS
	#ifdef IE_DLL
		#ifdef IE_BUILD_DLL
			#define IsoEngine_API __declspec(dllexport)
		#else
			#define IsoEngine_API __declspec(dllimport)
		#endif
	#else
		#define IsoEngine_API
	#endif
#else
	#error IsoEngine currently only supports Windows.
#endif

/* Debug Assertions */
// TODO: Fix Asserts

#ifdef IE_DEBUG_MODE
	#define IE_ENABLE_ASSERTS
#endif

/*
#ifdef IE_ENABLE_ASSERTS
	#define IE_APPLICATION_ASSERT(x, ...) { if(!(x)) {ISOLOGGER_FATAL(("IsoEngine Application Assertion Failed:", x, __VA_ARGS__); __debugbreak(); } }  // _debugbreak() is Windows OS only.
	#define IE_ENGINE_ASSERT(x, ...) { if(!(x)) {ISOLOGGER_FATAL(("IsoEngine Core Assertion Failed:", x, __VA_ARGS__); __debugbreak(); } }				// TODO: Implement assertion support for other OS.
#else
	#define IE_APPLICATION_ASSERT(x, ...)
	#define IE_ENGINE_ASSERT(x, ...)
#endif
*/

/* Bit-Field for EventCategory Class*/
#define BIT(x) (1 << x)

/* Event Binding Function */
#define IE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace IE
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;
}