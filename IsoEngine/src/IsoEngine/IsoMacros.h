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