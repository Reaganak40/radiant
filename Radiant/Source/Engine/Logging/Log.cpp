#include "pch.h"
#include "Log.h"

#include "Utils/Utils.h"

namespace rdt {

	constexpr size_t MaxLogs = 50;

	struct Log::Impl {
		std::shared_ptr<spdlog::logger> m_CoreLogger;
		std::shared_ptr<spdlog::logger> m_ClientLogger;
		std::shared_ptr< spdlog::sinks::ostream_sink_mt> m_sink;

		std::ostringstream m_log_oss;

		std::string m_logs[MaxLogs];
		Color m_logColors[MaxLogs];

		int logIndex;
		bool maxReached;

		Impl()
		{
			spdlog::set_pattern("%^[%T] %n: %v%$");

			m_sink = std::make_shared<spdlog::sinks::ostream_sink_mt>(m_log_oss);

			m_CoreLogger = std::make_shared<spdlog::logger>("Core", m_sink);
			m_CoreLogger->set_level(spdlog::level::trace);

			m_ClientLogger = std::make_shared<spdlog::logger>("Client", m_sink);
			m_ClientLogger->set_level(spdlog::level::trace);

			logIndex = 0;
			maxReached = false;
		}

		~Impl()
		{

		}

		void OnUpdate()
		{
			std::string stream = m_log_oss.str();
			std::vector<std::string> nLogs;

			Utils::Tokenize(stream, "\n", nLogs);

			for (std::vector<std::string>::reverse_iterator it = nLogs.rbegin(); it != nLogs.rend(); ++it) {
				m_logs[logIndex] = (*it);
				m_logColors[logIndex] = WHITE;
				logIndex = (logIndex + 1) % MaxLogs;
			}

			m_log_oss.str("");
			m_log_oss.clear();
		}

		bool GetLog(int index, std::string& msg, Color& msgColor)
		{
			if (index >= MaxLogs) {
				return false;
			}

			if (!maxReached && (index >= logIndex)) {
				return false;
			}

			index = (logIndex - index + MaxLogs) % MaxLogs;
			msg = m_logs[index];
			msgColor = m_logColors[index];

			return true;
		}
	};

	Log* Log::m_instance = nullptr;

	Log::Log()
		: m_impl(new Log::Impl)
	{
	}

	Log::~Log()
	{
		delete m_impl;
	}

	void Log::Initialize()
	{
		Destroy();
		m_instance = new Log;
	}

	void Log::Destroy()
	{
		if (m_instance != nullptr) {
			delete m_instance;
			m_instance = nullptr;
		}
	}

	void Log::Update()
	{
		m_instance->m_impl->OnUpdate();
	}

	inline std::shared_ptr<spdlog::logger>& Log::GetCoreLogger()
	{
		return m_instance->m_impl->m_CoreLogger;
	}

	inline std::shared_ptr<spdlog::logger>& Log::GetClientLogger()
	{
		return m_instance->m_impl->m_ClientLogger;
	}

	bool Log::GetLog(int index, std::string& msg, Color& msgColor)
	{
		return m_instance->m_impl->GetLog(index, msg, msgColor);
	}
}