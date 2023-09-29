#include "pch.h"
#include "Log.h"

namespace rdt {

	std::shared_ptr<spdlog::logger> Log::m_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::m_ClientLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		m_CoreLogger = spdlog::stderr_color_mt("Core");
		m_CoreLogger->set_level(spdlog::level::trace);

		m_ClientLogger = spdlog::stderr_color_mt("Client");
		m_CoreLogger->set_level(spdlog::level::trace);

	}

}