#pragma once
#include "Utils/UniqueID.h"

namespace rdt {
	enum PhysicalProperties {
		NoProperties = 0,	    // No special physical properties
		NoCollision = (1 << 0), // Do not detect or resolve any collisions
		ppBouncy    = (1 << 1), // When colliding with an object, bounce off according to its velocity
		ppNoGravity = (1 << 2), // Object is not affected by enabled gravitational forces
		DontResolve = (1 << 3), // Detect but do not resolve collisions
	};

	inline PhysicalProperties operator|(PhysicalProperties a, PhysicalProperties b)
	{
		return static_cast<PhysicalProperties>(static_cast<int>(a) | static_cast<int>(b));
	}

	inline PhysicalProperties& operator |=(PhysicalProperties& a, PhysicalProperties b)
	{
		return a = a | b;
	}

	inline PhysicalProperties operator^(PhysicalProperties a, PhysicalProperties b)
	{
		return static_cast<PhysicalProperties>(static_cast<int>(a) ^ static_cast<int>(b));
	}

	inline PhysicalProperties operator^=(PhysicalProperties& a, PhysicalProperties b)
	{
		return a = a ^ b;
	}
}
namespace rdt::core {

	// Physics Tags are used to sort objects of similar nature.
	class Ptag {
	private:
		UniqueID m_ID;
		PhysicalProperties m_properties;
	public:
		Ptag();
		~Ptag();

		UniqueID GetID();

		void SetProperties(PhysicalProperties nProperties);
		void RemoveProperties(PhysicalProperties rProperties);
		bool HasProperties(PhysicalProperties propertyQuery);
	};

	class PtagManager {
	private:
		PtagManager();
		~PtagManager();
		static PtagManager* m_instance;
		unsigned int m_idCounter;

		std::unordered_map<std::string, Ptag*> m_tags_by_name;
		std::unordered_map<UniqueID, Ptag*> m_tags_by_id;

	public:
		static void Initialize();
		static void Destroy();

		/*
			Use a string name to create a new ptag.
		*/
		static UniqueID CreateTag(const std::string& tagName) { return m_instance->CreateTagImpl(tagName); }

		/*
			Returns the physics tag assocated with the provided tag name.
		*/
		static UniqueID GetTagID(const std::string& tagName) { return m_instance->GetTagIDImpl(tagName); }

		/*
			Gets a reference to a ptag object through its ID
		*/
		static Ptag& GetTag(UniqueID tagID) { return m_instance->GetTagImpl(tagID); }

	private:
		UniqueID CreateTagImpl(const std::string& tagName);
		UniqueID GetTagIDImpl(const std::string& tagName);
		Ptag& GetTagImpl(UniqueID tagID);
	};
}