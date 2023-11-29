/***************************************************************/
/*  ECS/Entity.hpp                                             */
/* *************************************************************/
/*                 This file is a part of:                     */
/*                -- RADIANT GAME ENGINE --                    */
/*         https://github.com/Reaganak40/radiant               */
/***************************************************************/
/*            Copyright(c) 2023 Reagan Kelley                  */
/*                                                             */
/*  Permission  is  hereby  granted, free  of charge, to  any  */
/*  person obtaining a copy of this  software and  associated  */
/*  documentation  files(the  "Software"), to  deal  in   the  */
/*  Software without restriction,including without limitation  */
/*  the   rights   to  use,  copy,  modify,  merge,  publish,  */
/*  distribute,  sublicense,  and  /or  sell  copies  of  the  */
/*  Software,  and to permit persons to whom the  Software is  */
/*  furnished to do so, subject to the following conditions:   */
/*                                                             */
/*  The  above  copyright  notice  and this permission notice  */
/*  shall  be  included in all copies or substantial portions  */
/*  of the Software.                                           */
/*                                                             */
/*  THE  SOFTWARE  IS PROVIDED  "AS IS",  WITHOUT WARRANTY OF  */
/*  ANY KIND,  EXPRESS OR IMPLIED, INCLUDING  BUT NOT LIMITED  */
/*  TO THE  WARRANTIES  OF  MERCHANTABILITY,  FITNESS  FOR  A  */
/*  PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT  SHALL  */
/*  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,  */
/*  DAMAGES OR OTHER  LIABILITY,  WHETHER  IN  AN  ACTION  OF  */
/*  CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT  OF  OR IN  */
/*  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS  */
/*  IN THE SOFTWARE.                                           */
/***************************************************************/

#pragma once

/***************************************************************
* Headers
***************************************************************/
#include <Radiant/ECS/Export.hpp>
#include <Radiant/ECS/ECSTypes.hpp>
#include <Radiant/System/Vector.hpp>
#include <Radiant/Logger.hpp>

/***************************************************************
* Forward Declarations
***************************************************************/
namespace rdt {
	class IComponentArray {
	public:
		virtual ~IComponentArray() = default;

		virtual void  RemoveData(EntityID eID) = 0;
		virtual void* GetDataPtr(EntityID eID) = 0;
		virtual bool  IsEntityDisabled(EntityID eID) = 0;
	};

	template <typename T>
	class Component : public IComponentArray {
	private:
		vector<EntityID> m_entities;
		vector<T> m_data;

		// cache disabled entity data
		vector<EntityID> m_disabled_entities;
		vector<T> m_disabled_data;
	public:

		//Adds data to the component array belonging to the prescribed entity
		void InsertData(EntityID eID, const T& data) {
			if (HasEntity(eID) || IsEntityDisabled(eID)) {
				RDT_CORE_WARN("Component - Tried to insert duplicate entity [eID: {}]", eID);
				return;
			}

			m_entities.push_back(eID);
			m_data.push_back(data);
		}

		// Removes the data belonging to this entity
		void RemoveData(Entity eID) override final {

			auto it = m_entities.find(eID);
			if (it == m_entities.end()) {
				return;
			}

			size_t index = it - m_entities.begin();
			m_entities.erase(index);
			m_data.erase(index);
		}

		// Disables access to this entity's component data, effectively
		// muting its signature. Useful when one wants to temporarily
		// remove a component from an entity without losing its data.
		void DisableEntity(EntityID entity)
		{
			auto it = m_entities.find(eID);
			if (it == m_entities.end()) {
				return;
			}

			if (m_disabled_entities.find(entity) != m_disabled_entities.end())
			{
				RDT_CORE_WARN("Component - Tried to disable an already disabled entity [eID: {}]", entity);
				return;
			}

			size_t index = it - m_entities.begin();
			T data = m_data[index];
			m_entities.erase(index);
			m_data.erase(index);

			m_disabled_entities.push_back(entity);
			m_disabled_data.push_back(data);
		}

		// Re-enables an entity's component that has previously been disabled,
		// allowing systems to once again access its data.
		void EnableEntity(EntityID entity)
		{
			auto it = m_disabled_entities.find(eID);
			if (it == m_disabled_entities.end()) {
				return;
			}

			size_t index = it - m_disabled_entities.begin();
			T data = m_disabled_data[index];
			m_disabled_entities.erase(index);
			m_disabled_data.erase(index);

			m_entities.push_back(entity);
			m_data.push_back(data);
		}

		T& GetData(EntityID eID) {
			return m_data[m_entity_map.at(eID)];
		}

		bool HasEntity(EntityID eID) {
			return m_entities.find(eID) != m_entities.end();
		}

		bool IsEntityDisabled(EntityID eID) override final {
			return m_disabled_entities.find(eID) != m_disabled_entities.end();
		}

		// Returns a void pointer to the data component for the provided entity,
		// regardless of if it is enabled or not. Returns nullptr if not found.
		void* GetDataPtr(Entity eID) override final {

			if ((auto it = m_entities.find(eID)) != m_entities.end())
			{
				size_t index = it - m_entities.begin();
				return &m_data[index];
			}
			
			if ((auto it = m_disabled_entities.find(eID)) != m_disabled_entities.end())
			{
				size_t index = it - m_disabled_entities.begin();
				return &m_disabled_data[index];
			}

			return nullptr;
		}
	};
}