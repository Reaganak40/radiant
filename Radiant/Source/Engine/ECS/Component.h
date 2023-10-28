#pragma once
#include "Entity.h"
#include "Logging/Log.h"

namespace rdt {

	class IComponentArray {
		virtual ~IComponentArray() = default;
	};

	template <typename T>
	class Component : public IComponentArray {
	private:
		
		std::unordered_map<EntityID, size_t> m_entity_map;

		std::vector<EntityID> m_entities;
		std::vector<T> m_data;
	public:

		/*
			Adds data to the component array while tracking the associated
			entity.
		*/
		void InsertData(EntityID eID, T data) {
			if (m_entity_map.find(eID) != m_entity_map.end()) {
				RDT_CORE_WARN("Component - Tried to insert duplicate entity [eID: {}]", eID);
				return;
			}

			m_entity_map[eID] = m_data.size();
			m_entities.push_back(eID);
			m_data.push_back(data);
		}

		/*
			Removes the data from this component data array that is associated with this
			Entity.
		*/
		void RemoveData(EntityID eID) {
			if (m_entity_map.find(eID) == m_entity_map.end()) {
				RDT_CORE_WARN("Component - Could not remove data, entity not found [eID: {}]", eID);
				return;
			}

			size_t index = m_entity_map.at(eID);
			m_data.erase(index);
			m_entities.erase(index);
			m_entity_map.erase(eID);

			// Update the data index location of all entities using this component
			for (int i = index; i < m_entities.size(); i++) {
				m_entity_map.at(m_entities[i]) = i;
			}
		}

		T& GetData(EntityID eID) {
			return m_data[m_entity_map.at(eID)];
		}
	};
}