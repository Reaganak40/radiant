#pragma once
#include "logger_api.h"

// Forward Declarations
namespace rdt {
	class Color;
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

		static void CoreLog(LogLevel level, ...);
		static void ClientLog(LogLevel level, ...);

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
	};
}

// Core Logging Macros
#define RDT_CORE_TRACE(...) ::rdt::logger::Log::CoreLog(::rdt::logger::L_TRACE, __VA_ARGS__);
#define RDT_CORE_INFO(...)  ::rdt::logger::Log::CoreLog(::rdt::logger::L_INFO, __VA_ARGS__);
#define RDT_CORE_WARN(...)  ::rdt::logger::Log::CoreLog(::rdt::logger::L_WARNING, __VA_ARGS__);
#define RDT_CORE_ERROR(...) ::rdt::logger::Log::CoreLog(::rdt::logger::L_ERROR, __VA_ARGS__);
#define RDT_CORE_FATAL(...) ::rdt::logger::Log::CoreLog(::rdt::logger::L_CRITICAL, __VA_ARGS__);

// Client Logging Macros
#define RDT_TRACE(...) ::rdt::logger::Log::ClientLog(::rdt::logger::L_TRACE, __VA_ARGS__);
#define RDT_INFO(...)  ::rdt::logger::Log::ClientLog(::rdt::logger::L_INFO, __VA_ARGS__);
#define RDT_WARN(...)  ::rdt::logger::Log::ClientLog(::rdt::logger::L_WARNING, __VA_ARGS__);
#define RDT_ERROR(...) ::rdt::logger::Log::ClientLog(::rdt::logger::L_ERROR, __VA_ARGS__);
#define RDT_FATAL(...) ::rdt::logger::Log::ClientLog(::rdt::logger::L_CRITICAL, __VA_ARGS__);

