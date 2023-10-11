#include "pch.h"
#include "Ptag.h"

namespace rdt::core {

	Ptag::Ptag()
		: m_ID(GetUniqueID()), m_properties(NoProperties)
	{
	}

	Ptag::~Ptag()
	{
		FreeUniqueID(m_ID);
	}

	UniqueID Ptag::GetID()
	{
		return m_ID;
	}

	void Ptag::SetProperties(PhysicalProperties nProperties)
	{
		m_properties |= nProperties;
	}

	void Ptag::RemoveProperties(PhysicalProperties rProperties)
	{
		m_properties ^= rProperties;
	}

	bool Ptag::HasProperties(PhysicalProperties propertyQuery)
	{
		return bool(m_properties & propertyQuery);
	}

	// ============================================================

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

	UniqueID PtagManager::CreateTagImpl(const std::string& tagName)
	{
		if (m_tags_by_name.find(tagName) == m_tags_by_name.end()) {
			Ptag* tag;
			m_tags_by_name[tagName] = (tag = new Ptag);
			m_tags_by_id[tag->GetID()] = tag;
		}

		return m_tags_by_name.at(tagName)->GetID();
	}

	UniqueID PtagManager::GetTagIDImpl(const std::string& tagName)
	{
		if (m_tags_by_name.find(tagName) == m_tags_by_name.end()) {
			return 0;
		}
		return m_tags_by_name.at(tagName)->GetID();
	}
	Ptag& PtagManager::GetTagImpl(UniqueID tagID)
	{
		return (*m_tags_by_id.at(tagID));
	}
}