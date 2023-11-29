#include "pch.h"
#include <Radiant/Scene/SceneManager.hpp>
#include <Radiant/Scene/Scene.hpp>
#include "SceneManagerImpl.hpp"

#include <Core/Utils/Error.hpp>

static rdt::Scene* m_current_scene = nullptr;

void rdt::scene::SceneManager::Initialize()
{
	SceneManagerImpl::Get();
}

void rdt::scene::SceneManager::Destroy()
{
	SceneManagerImpl::Destroy();
}

void rdt::scene::SceneManager::SetCurrentScene(const char* sceneName)
{
	// do this check so scenes cannot change to undefined scenes
	Scene* nScene = SceneManagerImpl::Get().SetScene(sceneName);
	if (nScene != nullptr) {
		m_current_scene = nScene;
	}
}

rdt::Scene* rdt::scene::SceneManager::GetCurrentScene()
{
	return m_current_scene;
}

void rdt::scene::SceneManager::OnEndFrame()
{
	if (SceneManagerImpl::Get().OnEndFrame() == RDT_SCENE_CHANGED) {
		m_current_scene = SceneManagerImpl::Get().GetCurrentScene();
	}
}
