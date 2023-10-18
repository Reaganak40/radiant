#pragma once
#include "pch.h"
#include "Core.h"

#include "Utils/Color.h"

namespace rdt {
	
	class RADIANT_API Log {
	private:
		struct Impl;
		Impl* m_impl;

		Log();
		~Log();
		static Log* m_instance;

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

		static std::shared_ptr<spdlog::logger>& GetCoreLogger();
		static std::shared_ptr<spdlog::logger>& GetClientLogger();
		static bool GetLog(int index, std::string& msg, Color& msgColor);
	};
}

// Core Logging Macros
#define RDT_CORE_TRACE(...) ::rdt::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define RDT_CORE_INFO(...)  ::rdt::Log::GetCoreLogger()->info(__VA_ARGS__)
#define RDT_CORE_WARN(...)  ::rdt::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define RDT_CORE_ERROR(...) ::rdt::Log::GetCoreLogger()->error(__VA_ARGS__)
#define RDT_CORE_FATAL(...) ::rdt::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client Logging Macros
#define RDT_TRACE(...) ::rdt::Log::GetClientLogger()->trace(__VA_ARGS__)
#define RDT_INFO(...)  ::rdt::Log::GetClientLogger()->info(__VA_ARGS__)
#define RDT_WARN(...)  ::rdt::Log::GetClientLogger()->warn(__VA_ARGS__)
#define RDT_ERROR(...) ::rdt::Log::GetClientLogger()->error(__VA_ARGS__)
#define RDT_FATAL(...) ::rdt::Log::GetClientLogger()->critical(__VA_ARGS__)

