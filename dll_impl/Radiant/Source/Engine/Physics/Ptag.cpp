#include "pch.h"
#include "Ptag.h"

namespace rdt::core {
	PtagManager* PtagManager::m_instance = nullptr;

	PtagManager::PtagManager()
		: m_idCounter(0)
	{
	}

	PtagManager::~PtagManager()
	{
	}

	void PtagManager::Initialize()
	{
		Destroy();
		m_instance = new PtagManager;
	}

	void PtagManager::Destroy()
	{
		if (m_instance != nullptr) {
			delete m_instance;
			m_instance = nullptr;
		}
	}

	Ptag PtagManager::CreateTagImpl(const std::string& tagName)
	{
		if (m_tags.find(tagName) != m_tags.end()) {
			return m_tags.at(tagName);
		}

		m_tags[tagName] = ++m_idCounter;
		return m_idCounter;
	}

	Ptag PtagManager::GetTagImpl(const std::string& tagName)
	{
		if (m_tags.find(tagName) == m_tags.end()) {
			return 0;
		}

		return m_tags.at(tagName);
	}
}