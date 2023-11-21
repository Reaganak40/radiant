#pragma once

#ifdef RDT_PLATFORM_WINDOWS
	#ifdef LOGGER_BUILD_DLL
		#define LOGGER_API __declspec(dllexport)
#else
	#define LOGGER_API __declspec(dllimport)
#endif // LOGGER_BUILD_DLL
#else
	#error Unsupported Platformn - Windows Only
#endif // !RDT_PLATFORM_WINDOWS

