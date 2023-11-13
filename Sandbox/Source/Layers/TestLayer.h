#pragma once
#include <Radiant.h>

class TestLayer : public rdt::Layer {
private:
public:

	TestLayer(const std::string& alias = "TestLayer");
	~TestLayer();

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
	void OnRenderUpdate() override;

};
