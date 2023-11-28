#pragma once

#ifdef RDT_PLATFORM_WINDOWS
	#ifdef EDITOR_BUILD_DLL
		#define EDITOR_API __declspec(dllexport)
	#else
	#define EDITOR_API __declspec(dllimport)
	#endif // RADIANT_BUILD_DLL
#else
	#error Unsupported Platformn - Windows Only
#endif // !RDT_PLATFORM_WINDOWS
