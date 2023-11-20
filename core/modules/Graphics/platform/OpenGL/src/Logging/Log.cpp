#include "pch.h"
#include "Log.h"

namespace glCore {

	std::shared_ptr<spdlog::logger> Log::m_CoreLogger;
	bool Log::did_init = false;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		m_CoreLogger = spdlog::stderr_color_mt("glCore");
		m_CoreLogger->set_level(spdlog::level::trace);
	}

	bool glCore::Log::IsInitialized()
	{
		return did_init;;
	}

}