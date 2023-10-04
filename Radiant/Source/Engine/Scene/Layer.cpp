#include "pch.h"
#include "Layer.h"
#include "SceneManager.h"

namespace rdt {

	Layer::Layer()
		: m_ID(GetUniqueID())
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

}