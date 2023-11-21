#pragma once

#ifdef RDT_PLATFORM_WINDOWS
	#ifdef UTILS_BUILD_DLL
		#define UTILS_API __declspec(dllexport)
		#define UTILS_EXTERN 
	#else
		#define UTILS_API __declspec(dllimport)
		#define UTILS_EXTERN extern
	#endif // RADIANT_BUILD_DLL
#else
	#error Unsupported Platformn - Windows Only
#endif // !RDT_PLATFORM_WINDOWS
