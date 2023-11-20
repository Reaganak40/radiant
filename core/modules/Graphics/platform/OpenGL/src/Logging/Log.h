#pragma once
#include "Core.h"

namespace glCore {

	class Log {
	private:
		static bool did_init;
	public:
		/*
			Initialize the logging system for the engine core and the client.
		*/
		static void Init();

		static bool IsInitialized();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return m_CoreLogger; }

	private:

#pragma warning(push)
#pragma warning(disable: 4251)
		static std::shared_ptr<spdlog::logger> m_CoreLogger;
		static std::shared_ptr<spdlog::logger> m_ClientLogger;
#pragma warning(pop)

	};
}

// Core Logging Macros
#ifdef GL_CORE_DEBUG
	#define GL_CORE_TRACE(...) ::glCore::Log::GetCoreLogger()->trace(__VA_ARGS__)
	#define GL_CORE_INFO(...)  ::glCore::Log::GetCoreLogger()->info(__VA_ARGS__)
	#define GL_CORE_WARN(...)  ::glCore::Log::GetCoreLogger()->warn(__VA_ARGS__)
	#define GL_CORE_ERROR(...) ::glCore::Log::GetCoreLogger()->error(__VA_ARGS__)
	#define GL_CORE_FATAL(...) ::glCore::Log::GetCoreLogger()->critical(__VA_ARGS__)
#else
	#define GL_CORE_TRACE(...)
	#define GL_CORE_INFO(...)
	#define GL_CORE_WARN(...)
	#define GL_CORE_ERROR(...)
	#define GL_CORE_FATAL(...)
#endif