#pragma once

#ifdef RDT_PLATFORM_WINDOWS
	#ifdef RDT_BUILD_DLL
		#define RADIANT_API __declspec(dllexport)
		#define API_EXTERN 
	#else
		#define RADIANT_API __declspec(dllimport)
		#define API_EXTERN extern
	#endif // RDT_BUILD_DLL
	
	#define RDT_USE_OPENGL

#define EXPORT_STL(CLASS) API_EXTERN template class RADIANT_API CLASS
#define STL_STRING std::basic_string<char, std::char_traits<char>, std::allocator<char>>
#else
	#error Unsupported Platformn - Windows Only
#endif // !RDT_PLATFORM_WINDOWS
