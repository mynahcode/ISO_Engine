#pragma once

#include <memory>

/* User Platform Detection Macros */
#ifdef _WIN32
	/* Windows x64/x86 Platforms */
	#ifdef _WIN64
		/* Windows x64 Platforms */
		#define _IE_PLATFORM_WINDOWS

	#else
		/* Windows x86 Platforms */
		#error "x86 Platforms are not supported by IsoEngine!"
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetCondtionals.h>
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulators are not supported by IsoEngine!"
	#elif TARGET_OS_IPHONE == 1
		#define _IE_PLATFORM_IOS
		#error "IOS platforms are not supported by IsoEngine!"
	#elif TARGET_OS_MAC == 1
		#define _IE_PLATFORM_MACOS
		#error "MacOS is not supported by IsoEngine yet!"
	#else
		#error "Unknown Apple platform!"
	#endif
#elif defined(__ANDROID__)
	#define _IE_PLATFORM_ANDROID
	#error "Android is not yet supported by IsoEngine!"
#elif defined(__linux__)
	#define _IE_PLATFORM_LINUX
	#error "Linux is not yet supported by IsoEngine!"
#else 
	#error "Unknown user platform!"
#endif

/* Debug Assertions */
// TODO: Fix Asserts

#ifdef _IE_DEBUG_MODE
	#define IE_ENABLE_ASSERTS
#endif

/* Bit-Field for EventCategory Class*/
#define BIT(x) (1 << x)

/* Event Binding Function */
#define IE_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace IE
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ...Args>
	constexpr Scope<T> CreateScope(Args&& ...args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ...Args>
	constexpr Ref<T> CreateRef(Args&& ...args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}