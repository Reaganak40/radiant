#include "pch.h"
#include "Scene.h"
#include "SceneManager.h"

struct rdt::scene::Scene::Impl {

	SceneID m_ID;
	std::string m_name;

	Impl()
	{
	}

	Impl()
	{
	}
};

// =============================================
rdt::scene::Scene::Scene()
	: m_impl(new Scene::Impl)
{
}

rdt::scene::Scene::~Scene()
{
	delete m_impl;
}

rdt::SceneID rdt::scene::Scene::GetID()
{
	return m_impl->m_ID;
}

const char* rdt::scene::Scene::GetName()
{
	return m_impl->m_name.c_str();
}

void rdt::scene::Scene::SetSceneID(SceneID nID)
{
	m_impl->m_ID = nID;
}

void rdt::scene::Scene::SetSceneName(const char* nName)
{
	m_impl->m_name = nName;
}

rdt::SceneID rdt::scene::RegisterScene(const char* sceneName, Scene* scene)
{
	return SceneManager::RegisterScene(sceneName, scene);
}

rdt::SceneID rdt::scene::GetSceneID(const char* sceneName)
{
	return SceneManager::GetSceneID(sceneName);
}

void rdt::scene::SetScene(const char* sceneName)
{

}

void rdt::scene::SetScene(SceneID sceneID)
{
}
