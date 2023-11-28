/***************************************************************/
/*  Logger/Export.hpp                                          */
/* *************************************************************/
/*                 This file is a part of:                     */
/*                -- RADIANT GAME ENGINE --                    */
/*         https://github.com/Reaganak40/radiant               */
/***************************************************************/
/*            Copyright(c) 2023 Reagan Kelley                  */
/*                                                             */
/*  Permission  is  hereby  granted, free  of charge, to  any  */
/*  person obtaining a copy of this  software and  associated  */
/*  documentation  files(the  "Software"), to  deal  in   the  */
/*  Software without restriction,including without limitation  */
/*  the   rights   to  use,  copy,  modify,  merge,  publish,  */
/*  distribute,  sublicense,  and  /or  sell  copies  of  the  */
/*  Software,  and to permit persons to whom the  Software is  */
/*  furnished to do so, subject to the following conditions:   */
/*                                                             */
/*  The  above  copyright  notice  and this permission notice  */
/*  shall  be  included in all copies or substantial portions  */
/*  of the Software.                                           */
/*                                                             */
/*  THE  SOFTWARE  IS PROVIDED  "AS IS",  WITHOUT WARRANTY OF  */
/*  ANY KIND,  EXPRESS OR IMPLIED, INCLUDING  BUT NOT LIMITED  */
/*  TO THE  WARRANTIES  OF  MERCHANTABILITY,  FITNESS  FOR  A  */
/*  PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT  SHALL  */
/*  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,  */
/*  DAMAGES OR OTHER  LIABILITY,  WHETHER  IN  AN  ACTION  OF  */
/*  CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT  OF  OR IN  */
/*  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS  */
/*  IN THE SOFTWARE.                                           */
/***************************************************************/

#pragma once

/***************************************************************
* Headers
***************************************************************/
#include <Radiant/Logger/Export.hpp>

#ifndef RDT_DISABLE_LOGGING
// spdlog is a header-only library that relies on templated
// variadic functions, making it TERRIBLY incapatible with
// traditional implementation hiding patterns. So, the library
// must be available by the module or client to use it.
//
// if library not found, logging is disabled.
#include <spdlog/spdlog.h>

/***************************************************************
* Forward Declarations
***************************************************************/
namespace rdt {
	struct Vec4f;
}

namespace rdt {
	
	enum LogLevel {
		L_INFO,
		L_TRACE,
		L_WARNING,
		L_ERROR,
		L_CRITICAL,
	};

	class LOGGER_API Log {
	private:
		struct Impl;
		static Impl* m_impl;

	public:
		/*
			Initialize the logging system for the engine core and the client.
		*/
		static void Initialize();

		/*
			Destroys the logging instance
		*/
		static void Destroy();

		/*
			Updates the list of cached logs
		*/
		static void Update();

		static spdlog::logger& GetCoreLogger();
		static spdlog::logger& GetClientLogger();

		/*
			Gets a log message fro mthe log buffer at the given index. If no
			log found at the given index returns nullptr. Changes the msgColor
			reference to the intended text color of the log.
		*/
		static const char* GetLog(int index, Vec4f& msgColor);

		/*
			Returns the number of logs in the log buffer
		*/
		static size_t GetLogCount();

		/*
			Set the log color for logs at the given log level.
		*/
		static void SetLogColor(LogLevel level, Vec4f nColor);

		/*
			Activates the stdout buffer stream sink, allowing logging
			to the console window.
		*/
		static void log_to_stdout(bool yes_log = true);

	private:
		static Impl* GetImpl();
	};
}


#endif //RDT_DISABLE_LOGGING


/***************************************************************
* No logging available -- delete functions
***************************************************************/
#ifdef RDT_DISABLE_LOGGING
	// Core Logging Macros
	#define RDT_CORE_TRACE(...)
	#define RDT_CORE_INFO(...)
	#define RDT_CORE_WARN(...)
	#define RDT_CORE_ERROR(...)
	#define RDT_CORE_FATAL(...)

	// Client Logging Macros
	#define RDT_TRACE(...)
	#define RDT_INFO(...)
	#define RDT_WARN(...)
	#define RDT_ERROR(...)
	#define RDT_FATAL(...)
#else

/***************************************************************
* Enable core logger if using editor / in debug mode
***************************************************************/
	#ifdef RDT_USE_EDITOR
		// Core Logging Macros
		#define RDT_CORE_TRACE(...) ::rdt::Log::GetCoreLogger().trace(__VA_ARGS__)
		#define RDT_CORE_INFO(...)  ::rdt::Log::GetCoreLogger().info(__VA_ARGS__)
		#define RDT_CORE_WARN(...)  ::rdt::Log::GetCoreLogger().warn(__VA_ARGS__)
		#define RDT_CORE_ERROR(...) ::rdt::Log::GetCoreLogger().error(__VA_ARGS__)
		#define RDT_CORE_FATAL(...) ::rdt::Log::GetCoreLogger().critical(__VA_ARGS__)
	#else
		// Core Logging Macros
		#define RDT_CORE_TRACE(...)
		#define RDT_CORE_INFO(...)
		#define RDT_CORE_WARN(...)
		#define RDT_CORE_ERROR(...)
		#define RDT_CORE_FATAL(...)
	#endif
	

/***************************************************************
* Enable client logger if not in publish mode
***************************************************************/
	// Client Logging Macros
	#define RDT_TRACE(...) ::rdt::logger::Log::GetClientLogger().trace(__VA_ARGS__)
	#define RDT_INFO(...)  ::rdt::logger::Log::GetClientLogger().info(__VA_ARGS__)
	#define RDT_WARN(...)  ::rdt::logger::Log::GetClientLogger().warn(__VA_ARGS__)
	#define RDT_ERROR(...) ::rdt::logger::Log::GetClientLogger().error(__VA_ARGS__)
	#define RDT_FATAL(...) ::rdt::logger::Log::GetClientLogger().critical(__VA_ARGS__)
#endif