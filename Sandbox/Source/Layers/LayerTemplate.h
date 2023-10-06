#pragma once
#include <Radiant.h>

class TEMPLATENAME_LAYER : public rdt::Layer {
private:
public:

	TEMPLATENAME_LAYER(const std::string& alias = "TEMPLATENAME_LAYER");
	~TEMPLATENAME_LAYER();

	/*
		Function called when this layer is binded to a Scene, entering
		the game loop.
	*/
	void OnAttach() override;

	/*
		Function called when a layer is about to be released, not active.
	*/
	void OnDetach() override;

	/*
		Callback function when the MessageBus is delivering a message to
		this object.
	*/
	void OnMessage(rdt::Message msg) override;

	/*
		Function called when the layer is active and the host scene
		calls ProcessInput.
	*/
	void OnProcessInput(const float deltaTime) override;

	/*
		Function called when the layer is active and the host scene
		calls FinalUpdate()
	*/
	void OnFinalUpdate() override;

	/*
		Function called when the layer is active and the host scene
		calls Render()
	*/
	void OnRender() override;

};