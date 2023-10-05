#include "pch.h"
#include "Layer.h"
#include "SceneManager.h"
#include "Physics/Physics.h"

namespace rdt {

	Layer::Layer()
		: m_ID(GetUniqueID()),  m_attached(false)
	{
	}
	Layer::~Layer()
	{
		for (auto& object : m_game_objects) {
			delete object;
		}

		for (auto& gui: m_GUIs) {
			delete gui;
		}

		FreeUniqueID(m_ID);
	}

	GameObject** Layer::GetGameObjects(unsigned int* numObjects)
	{
		*numObjects = m_game_objects.size();
		return m_game_objects.data();
	}

	void Layer::SetAttached(bool attach)
	{
		m_attached = attach;
	}

	void Layer::RunProcessInputQueue(const float deltaTime)
	{
		for (auto& object : m_game_objects) {
			object->OnProcessInput(deltaTime);
		}

		for (auto& gui : m_GUIs) {
			gui->OnUpdate(deltaTime);
		}
	}
	
	void Layer::RunFinalUpdateQueue()
	{
		for (auto& object : m_game_objects) {
			object->OnFinalUpdate();
		}
	}

	void Layer::RunRenderQueue()
	{
		for (auto& object : m_game_objects) {
			object->OnRender();
		}
	}

	void Layer::CreateNewRealm()
	{
		m_realms.push_back(Physics::CreateRealm());
	}

}
