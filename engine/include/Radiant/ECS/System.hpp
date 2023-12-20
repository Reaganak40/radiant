/***************************************************************/
/*  ECS/System.hpp                                             */
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
#include <Radiant/ECS/ComponentManager.hpp>
#include <unordered_map>
#include <typeindex>
#include <string>

/***************************************************************
* Forward Declarations
***************************************************************/
namespace rdt {
	enum SystemUpdateType;
	namespace ecs {
		class EntityManager;
	}
}

namespace rdt::ecs {

	/***************************************************************
	* Base System Class
	***************************************************************/

	// Systems are used to instantiate unique proecures by the client.
	class System {
		// no dll interface (compile on client-side)
	private:

		std::unordered_map<ComponentID, IComponentArray*> in_use_components;
		Signature m_signature;
		std::vector<EntityID> m_entities;

	public:
		// The type determines at what update step the system is processed
		virtual SystemUpdateType GetType() = 0;

	protected:

		// Delcares the usage of a component for this system. This should
		// be called in a system's constructor to define the nature of 
		// the system.
		template<typename T>
		static void UseComponent()
		{
			ComponentID cID = GetComponentID<T>();
			ComponentSlot sigBit = ComponentManager::GetComponentSlot(cID);
			if (sigBit == RDT_NOT_REGISTERED_COMPONENT)
			{
				ComponentManager::RegisterComponent<T>();
				sigBit = ComponentManager::GetComponentSlot(cID);
			}

			in_use_components[cID] = ComponentManager::GetComponentArray(cID);
			m_signature.set(sigSlot, true);
		}

		// Gets the entity's component data. This should only be called
		// using delcared components and entities in the system's entity list.
		template<typename T>
		T& GetComponent(EntityID entity)
		{
			// this looks absolutely disgusting
			*static_cast<T*>(
				static_cast<ComponentData<T>*>(
					ComponentManager::GetComponentArray(GetComponentID<T>())
					)->GetDataPtr(entity)
				);
		}

		const std::vector<EntityID>& GetEntities()
		{
			return m_entities;
		}

		// To implement function that contains the logic for the system
		virtual void Update(float deltaTime = 0.0f) = 0;

	private:
		friend class ecs::SystemManager;

		Signature& GetSignature() { return m_signature; }
		void AddEntity(const EntityID nEntity) { m_entities.push_back(nEntity); }
	};
}

/***************************************************************
* Different System Classes: Used by client
***************************************************************/
namespace rdt {

	enum SystemUpdateType {
		SystemNAT,			// NOT A TYPE
		SystemProcessInput, // System for OnProcessInput()
		SystemWorldUpdate,  // System for OnWorldUpdate()
		SystemFinalUpdate,	// System for OnFinalUpdate()
		SystemRenderUpdate, // System for OnRender()
	};

	// =====================================================
	class ProcessInput : public ecs::System {
	public:
		ProcessInput() {}
		~ProcessInput() {}
		SystemUpdateType GetType() override final { return SystemProcessInput; }
		virtual void Update(float deltaTime = 0) { RDT_CORE_WARN("System::ProcessInput - virtual update called."); }
	};
	// =====================================================
	class WorldUpdate : public ecs::System {
	public:
		WorldUpdate() {}
		~WorldUpdate() {}
		SystemUpdateType GetType() override final { return SystemWorldUpdate; }
		virtual void Update(float deltaTime = 0) { RDT_CORE_WARN("System::WorldUpdate - virtual update called."); }
	};
	// =====================================================
	class FinalUpdate : public ecs::System {
	public:
		FinalUpdate() {}
		~FinalUpdate() {}
		SystemUpdateType GetType() override final { return SystemFinalUpdate; }
		virtual void Update(float deltaTime = 0) { RDT_CORE_WARN("System::FinalUpdate - virtual update called."); }
	};
	// =====================================================
	class RenderUpdate : public ecs::System {
	public:
		RenderUpdate() {}
		~RenderUpdate() {}
		SystemUpdateType GetType() override final { return SystemRenderUpdate; }
		virtual void Update(float deltaTime = 0) { RDT_CORE_WARN("System::RenderUpdate - virtual update called."); }
	};
	// =====================================================
}

namespace rdt::ecs {

	using SystemID = std::type_index;
	template<typename T>
	SystemID GetSystemID()
	{
		static_assert(std::is_base_of<System, T>::value);
		return std::type_index(typeid(T));
	}
}