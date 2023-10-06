#include "SceneTemplate.h"

using namespace rdt;

TEMPLATENAME_SCENE::TEMPLATENAME_SCENE(const std::string& alias)
{
	RegisterToMessageBus(alias);
}

TEMPLATENAME_SCENE::~TEMPLATENAME_SCENE()
{
}

void TEMPLATENAME_SCENE::OnRegister()
{
}

void TEMPLATENAME_SCENE::OnBind()
{

	/* Attaches all layers belonging to this scene. */
	Scene::OnBind();
}

void TEMPLATENAME_SCENE::OnRelease()
{

	/* Detaches all layers belonging to this scene. */
	Scene::OnRelease();
}

void TEMPLATENAME_SCENE::OnMessage(Message msg)
{
	switch (msg.type) {
	default:
		break;
	}
}

void TEMPLATENAME_SCENE::OnProcessInput(const float deltaTime)
{

	/* Calls OnProcessInput on all attached layers. */
	Scene::OnProcessInput(deltaTime);
}

void TEMPLATENAME_SCENE::OnFinalUpdate()
{

	/* Calls OnFinalUpdate on all attached layers. */
	Scene::OnFinalUpdate();
}

void TEMPLATENAME_SCENE::OnRender()
{

	/* Calls OnRender om all attached layers. */
	Scene::OnRender();
}
