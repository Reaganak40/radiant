#include "pch.h"
#include "Layer.h"
#include "SceneManager.h"
#include "Physics/Physics.h"
#include "Graphics/Renderer.h"

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

	void Layer::BindAll()
	{
		for (auto& object : m_game_objects) {
			object->OnBind();
		}

		for (auto& gui : m_GUIs) {
			Renderer::AttachGui(gui);
		}
	}

	void Layer::ReleaseAll()
	{
		for (auto& object : m_game_objects) {
			object->OnRelease();
		}

		for (auto& gui : m_GUIs) {
			Renderer::DetachGui(gui);
		}
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
