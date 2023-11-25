#pragma once
#ifdef RDT_PLATFORM_WINDOWS
	#ifdef SCENE_BUILD_DLL
		#define SCENE_API __declspec(dllexport)
	#else
		#define SCENE_API __declspec(dllimport)
	#endif // SCENE_BUILD_DLL
#else
	#error Unsupported Platform - Windows Only
#endif // !RDT_PLATFORM_WINDOWS
