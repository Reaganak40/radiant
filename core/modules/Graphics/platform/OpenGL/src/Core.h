#pragma once

#ifdef GL_CORE_PLATFORM_WINDOWS
	#ifdef GL_CORE_BUILD_DLL
		#define GL_CORE_API __declspec(dllexport)
		#define API_EXTERN 
	#else
		#define GL_CORE_API __declspec(dllimport)
		#define API_EXTERN extern
	#endif // GL_CORE_BUILD_DLL

#define EXPORT_STL(CLASS) API_EXTERN template class GL_CORE_API CLASS
#define STL_STRING std::basic_string<char, std::char_traits<char>, std::allocator<char>>
#else
	#error Unsupported Platformn - Windows Only
#endif // !GL_CORE_PLATFORM_WINDOWS

