#include "LayerTemplate.h"

using namespace rdt;

TEMPLATENAME_LAYER::TEMPLATENAME_LAYER(const std::string& alias)
{
	RegisterToMessageBus(alias);
}

TEMPLATENAME_LAYER::~TEMPLATENAME_LAYER()
{
}

void TEMPLATENAME_LAYER::OnAttach()
{
	// TODO: Bind GameObjects and GUIs

}

void TEMPLATENAME_LAYER::OnDetach()
{
	// TODO: Unbind GameObjects and GUIs

}

void TEMPLATENAME_LAYER::OnMessage(rdt::Message msg)
{
	switch (msg.type) {
	default:
		break;
	}
}

void TEMPLATENAME_LAYER::OnProcessInput(const float deltaTime)
{

	/* 
		Calls ProcessInput on all binded game objects
		and GUIs.
	*/
	Layer::OnProcessInput(deltaTime);
}

void TEMPLATENAME_LAYER::OnFinalUpdate()
{

	/* Calls FinalUpdate on all binded game objects. */
	Layer::OnFinalUpdate();
}

void TEMPLATENAME_LAYER::OnRender()
{

	/* 
		Calls OnRender on all binded game objects
		and GUIs.
	*/
	Layer::OnRender();
}
