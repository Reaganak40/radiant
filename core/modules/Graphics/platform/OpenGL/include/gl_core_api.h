#pragma once
#ifdef RDT_PLATFORM_WINDOWS
	#ifdef GL_CORE_BUILD_DLL
	#define GL_CORE_API __declspec(dllexport)
#else
	#define GL_CORE_API __declspec(dllimport)
	#endif // GL_CORE_BUILD_DLL
#else
	#error Unsupported Platformn - Windows Only
#endif // !GL_CORE_PLATFORM_WINDOWS
