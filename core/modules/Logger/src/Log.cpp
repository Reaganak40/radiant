#include "pch.h"
#include "Log.h"

#include <Radiant/Utils.h>

namespace rdt::logger {

	constexpr size_t MaxLogs = 250;

	struct Log::Impl {
		std::shared_ptr<spdlog::logger> m_CoreLogger;
		std::shared_ptr<spdlog::logger> m_ClientLogger;
		std::shared_ptr< spdlog::sinks::ostream_sink_mt> m_sink;

		std::ostringstream m_log_oss;

		std::string m_logs[MaxLogs];
		Color m_logColors[MaxLogs];
		LogLevel m_log_levels[MaxLogs];
		Color m_level_color[5];

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

			m_level_color[L_INFO] = GREEN;
			m_level_color[L_TRACE] = WHITE;
			m_level_color[L_WARNING] = { 0.97f, 0.74f, 0.23f, 1.0f };
			m_level_color[L_ERROR] = RED;
			m_level_color[L_CRITICAL] = { 0.808f, 0.0f, 0.058f, 1.0f };
		}

		~Impl()
		{

		}

		void OnUpdate()
		{
			std::string stream = m_log_oss.str();
			std::vector<std::string> nLogs;

			rdt::utils::Tokenize(stream, "\n", nLogs);

			for (auto log : nLogs) {

				int end1 = 0;
				int start2 = 0;
				int i = 0;
				int bracketCount = 2;
				for (; i < log.size(); i++) {
					if (log[i] == ']') {
						bracketCount--;

						if (bracketCount == 0) {
							i++;
							break;
						}
					}
				}
				end1 = i;
				for (; i < log.size(); i++) {
					if (log[i] == ']') {
						i++;
						start2 = i;
						break;
					}
				}
				std::string level;
				log = StripLogLevel(log, level);

				LogLevel logLevel;
				if (level == "info") {
					logLevel = L_INFO;
				}
				else if (level == "warning") {
					logLevel = L_WARNING;
				}
				else if (level == "error") {
					logLevel = L_ERROR;
				}
				else if (level == "critical") {
					logLevel = L_CRITICAL;
				}
				else {
					logLevel = L_TRACE;
				}

				m_logs[logIndex] = log;
				m_log_levels[logIndex] = logLevel;
				m_logColors[logIndex] = m_level_color[logLevel];
				logIndex = (logIndex + 1) % MaxLogs;

				if (logIndex == 0) {
					maxReached = true;
				}
			}

			m_log_oss.str("");
			m_log_oss.clear();
		}

		const char* GetLog(int index, Color& msgColor)
		{
			if (index >= MaxLogs) {
				return nullptr;
			}

			if (!maxReached && (index >= logIndex)) {
				return nullptr;
			}

			index = (logIndex - index - 1 + MaxLogs) % MaxLogs;
			msgColor = m_logColors[index];

			return m_logs[index].c_str();
		}

		std::string StripLogLevel(const std::string& originalLog, std::string& outLevel)
		{
			int end1 = 0;
			int start2 = 0;
			int i = 0;
			int bracketCount = 2;
			for (; i < originalLog.size(); i++) {
				if (originalLog[i] == ']') {
					bracketCount--;

					if (bracketCount == 0) {
						i++;
						break;
					}
				}
			}
			end1 = i;
			for (; i < originalLog.size(); i++) {
				if (originalLog[i] == ']') {
					i++;
					start2 = i;
					break;
				}
			}
			
			outLevel = originalLog.substr(end1 + 2, start2 - (end1 + 2) - 1);
			return originalLog.substr(0, end1) + originalLog.substr(start2, originalLog.size() - start2);
		}
	};

	// ===============================================================
	Log::Impl* Log::m_impl = nullptr;

	void Log::Initialize()
	{
		Destroy();
		m_impl = new Log::Impl;
	}

	void Log::Destroy()
	{
		if (m_impl != nullptr) {
			delete m_impl;
			m_impl = nullptr;
		}
	}

	void Log::Update()
	{
		m_impl->OnUpdate();
	}

	void Log::CoreLog(LogLevel level, ...)
	{
		va_list args;
		va_start(args, level);

		switch (level) {
		case L_INFO:
			m_impl->m_CoreLogger->info(args);
			break;
		case L_TRACE:
			m_impl->m_CoreLogger->trace(args);
			break;
		case L_WARNING:
			m_impl->m_CoreLogger->warn(args);
			break;
		case L_ERROR:
			m_impl->m_CoreLogger->error(args);
			break;
		case L_CRITICAL:
			m_impl->m_CoreLogger->critical(args);
			break;
		}
	}

	void Log::ClientLog(LogLevel level, ...)
	{
		va_list args;
		va_start(args, level);

		switch (level) {
		case L_INFO:
			m_impl->m_ClientLogger->info(args);
			break;
		case L_TRACE:
			m_impl->m_ClientLogger->trace(args);
			break;
		case L_WARNING:
			m_impl->m_ClientLogger->warn(args);
			break;
		case L_ERROR:
			m_impl->m_CoreLogger->error(args);
			break;
		case L_CRITICAL:
			m_impl->m_ClientLogger->critical(args);
			break;
		}
	}

	const char* Log::GetLog(int index, Color& msgColor)
	{
		return m_impl->GetLog(index, msgColor);
	}

	size_t Log::GetLogCount()
	{
		if (m_impl->maxReached) {
			return MaxLogs;
		}
		
		return m_impl->logIndex;
	}

	void Log::SetLogColor(LogLevel level, Color nColor)
	{
		m_impl->m_level_color[level] = nColor;

		for (int i = 0; i < MaxLogs; i++) {
			if (m_impl->m_log_levels[i] == level) {
				m_impl->m_logColors[i] = nColor;
			}
		}
	}
}