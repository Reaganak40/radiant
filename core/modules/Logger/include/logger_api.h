#pragma once

#ifdef RDT_PLATFORM_WINDOWS
	#ifdef LOGGER_BUILD_DLL
		#define LOGGER_API __declspec(dllexport)
		#define API_EXTERN 
#else
	#define LOGGER_API __declspec(dllimport)
	#define API_EXTERN extern
#endif // RADIANT_BUILD_DLL
#else
	#error Unsupported Platformn - Windows Only
#endif // !RDT_PLATFORM_WINDOWS

#define GET_INTERFACE(interface_name) "../../src/" ## interface_name