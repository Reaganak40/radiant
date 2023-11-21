#pragma once

#ifdef RDT_PLATFORM_WINDOWS
	#ifdef RDT_BUILD_DLL
		#define RADIANT_API __declspec(dllexport)
		#define API_EXTERN 
	#else
		#define RADIANT_API __declspec(dllimport)
		#define API_EXTERN extern
	#endif // RADIANT_BUILD_DLL
#else
	#error Unsupported Platformn - Windows Only
#endif // !RDT_PLATFORM_WINDOWS
