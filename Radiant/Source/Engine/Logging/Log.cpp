#include "pch.h"
#include "Log.h"

#include "Utils/Utils.h"

namespace rdt {

	constexpr size_t MaxLogs = 250;

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

				Color logColor = WHITE;
				if (level == "info") {
					logColor = GREEN;
				}
				else if (level == "warning") {
					logColor = { 0.97f, 0.74f, 0.23f, 1.0f };
				}
				else if (level == "error") {
					logColor = RED;
				}
				else if (level == "critical") {
					logColor = { 0.808f, 0.0f, 0.058f, 1.0f };
				}

				m_logs[logIndex] = log;
				m_logColors[logIndex] = logColor;
				logIndex = (logIndex + 1) % MaxLogs;

				if (logIndex == 0) {
					maxReached = true;
				}
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

			index = (logIndex - index - 1 + MaxLogs) % MaxLogs;
			msg = m_logs[index];
			msgColor = m_logColors[index];

			return true;
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

	std::shared_ptr<spdlog::logger>& Log::GetCoreLogger()
	{
		return m_instance->m_impl->m_CoreLogger;
	}

	std::shared_ptr<spdlog::logger>& Log::GetClientLogger()
	{
		return m_instance->m_impl->m_ClientLogger;
	}

	size_t Log::GetLogCount()
	{
		if (m_instance->m_impl->maxReached) {
			return MaxLogs;
		}
		
		return m_instance->m_impl->logIndex;
	}

	bool Log::GetLog(int index, std::string& msg, Color& msgColor)
	{
		return m_instance->m_impl->GetLog(index, msg, msgColor);
	}
}