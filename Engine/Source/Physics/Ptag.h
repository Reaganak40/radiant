#pragma once

namespace rdt {

	// Physics Tags are used to sort objects of similar nature.
	typedef unsigned int Ptag;

	class PtagManager {
	private:
		PtagManager();
		~PtagManager();
		static PtagManager* m_instance;
		unsigned int m_idCounter;

		std::unordered_map<std::string, Ptag> m_tags;

	public:
		static void Initialize();
		static void Destroy();

		/*
			Use a string name to create a new ptag.
		*/
		static Ptag CreateTag(const std::string& tagName) { return m_instance->CreateTagImpl(tagName); }

		/*
			Returns the physics tag assocated with the provided tag name.
		*/
		static Ptag GetTag(const std::string& tagName) { return m_instance->GetTagImpl(tagName); }

	private:
		Ptag CreateTagImpl(const std::string& tagName);
		Ptag GetTagImpl(const std::string& tagName);
	};
}