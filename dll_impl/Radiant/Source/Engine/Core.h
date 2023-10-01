#pragma once

#ifdef RDT_PLATFORM_WINDOWS
	#ifdef RDT_BUILD_DLL
		#define RADIANT_API __declspec(dllexport)
	#else
		#define RADIANT_API __declspec(dllimport)
	#endif // RDT_BUILD_DLL
	
	#define RDT_USE_OPENGL
#else
	#error Unsupported Platformn - Windows Only
#endif // !RDT_PLATFORM_WINDOWS
