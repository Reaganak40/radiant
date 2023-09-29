#pragma once
#include "pch.h"
#include "Core.h"

namespace rdt {
	
	class RADIANT_API Log {
	private:
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return m_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return m_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> m_CoreLogger;
		static std::shared_ptr<spdlog::logger> m_ClientLogger;

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

