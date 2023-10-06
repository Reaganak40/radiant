#pragma once
#include <Radiant.h>

class MyBeautifulObject : public rdt::GameObject {
private:
public:

	MyBeautifulObject(const std::string& alias = "MyBeautifulObject");
	~MyBeautifulObject();

	/*
		Function called when the host layer binds this game object to
		the scene.
	*/
	void OnBind() override;

	/*
		Function called when the host layer unbinds this game object
		from the scene.
	*/
	void OnRelease() override;

	/*
		Callback function when the MessageBus is delivering a message to
		this object.
	*/
	void OnMessage(rdt::Message msg) override;

	/*
		Function called when the host layer calls OnProcessInput
	*/
	void OnProcessInput(const float deltaTime) override;

	/*
		Function called when the host layer calls OnFinalUpdate
	*/
	void OnFinalUpdate() override;

	/*
		Function called when the host layer calls OnRender.
	*/
	void OnRender() override;
};
