#pragma once
#include "logger_api.h"

// Forward Declarations
namespace rdt {
	class Color;
}

namespace spdlog {
	class logger;
}
namespace rdt::logger {
	
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

		static spdlog::logger* GetCoreLogger();
		static spdlog::logger* GetClientLogger();

		/*
			Gets a log message fro mthe log buffer at the given index. If no
			log found at the given index returns nullptr. Changes the msgColor
			reference to the intended text color of the log.
		*/
		static const char* GetLog(int index, Color& msgColor);

		/*
			Returns the number of logs in the log buffer
		*/
		static size_t GetLogCount();

		/*
			Set the log color for logs at the given log level.
		*/
		static void SetLogColor(LogLevel level, Color nColor);

		/*
			Activates the stdout buffer stream sink, allowing logging
			to the console window.
		*/
		static void log_to_stdout(bool yes_log = true);

	private:
		static Impl* GetImpl();
	};
}

// Core Logging Macros
#define RDT_CORE_TRACE(...) ::rdt::logger::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define RDT_CORE_INFO(...)  ::rdt::logger::Log::GetCoreLogger()->info(__VA_ARGS__)
#define RDT_CORE_WARN(...)  ::rdt::logger::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define RDT_CORE_ERROR(...) ::rdt::logger::Log::GetCoreLogger()->error(__VA_ARGS__)
#define RDT_CORE_FATAL(...) ::rdt::logger::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client Logging Macros
#define RDT_TRACE(...) ::rdt::logger::Log::GetClientLogger()->trace(__VA_ARGS__)
#define RDT_INFO(...)  ::rdt::logger::Log::GetClientLogger()->info(__VA_ARGS__)
#define RDT_WARN(...)  ::rdt::logger::Log::GetClientLogger()->warn(__VA_ARGS__)
#define RDT_ERROR(...) ::rdt::logger::Log::GetClientLogger()->error(__VA_ARGS__)
#define RDT_FATAL(...) ::rdt::logger::Log::GetClientLogger()->critical(__VA_ARGS__)

