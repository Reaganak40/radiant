#include "pch.h"
#include "Layer.h"
#include "SceneManager.h"
#include "Physics/Physics.h"
#include "Graphics/Renderer.h"

namespace rdt {

	struct Layer::Impl {
		UniqueID m_ID;
		bool m_attached;

		std::vector<GameObject*> m_game_objects;
		std::vector<GuiTemplate*> m_GUIs;
		std::vector<UniqueID> m_realms;
		std::vector<Entity> m_entities;

		Impl()
			: m_ID(GetUniqueID()), m_attached(false)
		{
		}

		~Impl()
		{
			for (auto& object : m_game_objects) {
				delete object;
			}

			for (auto& gui : m_GUIs) {
				delete gui;
			}

			for (auto entity : m_entities) {
				EntityManager::RemoveEntity(entity);
			}

			FreeUniqueID(m_ID);
		}

		GameObject** GetGameObjects(size_t* numObjects)
		{
			*numObjects = m_game_objects.size();
			return m_game_objects.data();
		}

		void BindAll()
		{
			for (auto& object : m_game_objects) {
				object->OnBind();
			}

			for (auto& gui : m_GUIs) {
				Renderer::AttachGui(gui);
			}
		}

		void ReleaseAll()
		{
			for (auto& object : m_game_objects) {
				object->OnRelease();
			}

			for (auto& gui : m_GUIs) {
				Renderer::DetachGui(gui);
			}
		}

		void RunProcessInputQueue(const float deltaTime)
		{
			for (auto& object : m_game_objects) {
				object->OnProcessInput(deltaTime);
			}

			for (auto& gui : m_GUIs) {
				gui->OnUpdate(deltaTime);
			}
		}

		void RunFinalUpdateQueue()
		{
			for (auto& object : m_game_objects) {
				object->OnFinalUpdate();
			}
		}

		void RunRenderQueue()
		{
			for (auto& object : m_game_objects) {
				object->OnRender();
			}
		}
	};

	// ==================================================================

	std::vector<GameObject*>& Layer::GetGameObjects()
	{
		return m_impl->m_game_objects;
	}

	std::vector<GuiTemplate*>& Layer::GetGUIs()
	{
		return m_impl->m_GUIs;
	}

	std::vector<UniqueID>& Layer::GetRealms()
	{
		return m_impl->m_realms;
	}

	void Layer::RegisterGameObject(GameObject* nGameObject)
	{
		m_impl->m_game_objects.push_back(nGameObject);
	}

	void Layer::RegisterGUI(GuiTemplate* nGUI)
	{
		m_impl->m_GUIs.push_back(nGUI);
	}

	void Layer::RegisterEntity(EntityDefinition* nEntity, const std::string& alias)
	{
		nEntity->SetEntityOwner(this);
		if (!alias.empty()) {
			nEntity->SetEntityAlias(alias);
		}

		m_impl->m_entities.push_back(EntityFactory::Create(nEntity)); // frees nEntity
	}

	Layer::Layer()
		: m_impl(new Layer::Impl)
	{
	}
	Layer::~Layer()
	{
		delete m_impl;
	}

	const UniqueID Layer::GetID()
	{
		return m_impl->m_ID;
	}

	void Layer::OnProcessInput(const float deltaTime)
	{
		m_impl->RunProcessInputQueue(deltaTime);
	}

	void Layer::OnFinalUpdate()
	{
		m_impl->RunFinalUpdateQueue();
	}

	void Layer::OnRender()
	{
		m_impl->RunRenderQueue();
	}

	bool Layer::IsAttached()
	{
		return m_impl->m_attached;
	}

	GameObject** Layer::GetGameObjects(size_t* numObjects)
	{
		return m_impl->GetGameObjects(numObjects);
	}

	const std::vector<Entity>& Layer::GetEntities()
	{
		return m_impl->m_entities;
	}

	void Layer::BindAll()
	{
		m_impl->BindAll();
	}

	void Layer::ReleaseAll()
	{
		m_impl->ReleaseAll();
	}

	void Layer::SetAttached(bool attach)
	{
		m_impl->m_attached = attach;
	}

	void Layer::CreateNewRealm()
	{
		m_impl->m_realms.push_back(Physics::CreateRealm());
	}

	void Layer::DestroyGameObject(MessageID mID)
	{
		bool found = false;
		for (std::vector<GameObject*>::iterator it = m_impl->m_game_objects.begin(); it != m_impl->m_game_objects.end(); ++it) {
			if ((*it)->GetMessageID() == mID) {
				if ((*it)->IsBinded()) {
					(*it)->OnRelease();
				}
				delete((*it));
				m_impl->m_game_objects.erase(it);
				found = true;
				break;
			}
		}

		if (!found) {
			RDT_CORE_WARN("Layer - Could not find game object to destroy with MessageID {}", mID);
		}
	}

}
