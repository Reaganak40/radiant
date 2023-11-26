#pragma once

#pragma once
#ifdef RDT_PLATFORM_WINDOWS
	#ifdef GRAPHICS_BUILD_DLL
		#define GRAPHICS_API __declspec(dllexport)
	#else
		#define GRAPHICS_API __declspec(dllimport)
	#endif // GL_CORE_BUILD_DLL
#else
	#error Unsupported Platform - Windows Only
#endif // !GL_CORE_PLATFORM_WINDOWS
