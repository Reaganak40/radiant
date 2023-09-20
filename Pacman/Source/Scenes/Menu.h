#pragma once
#include "Game.h"

class Menu : public rdt::Scene {
private:
public:
	Menu();
	~Menu();

	void OnRegister() override final;
	void OnBind() override final;
	void OnProcessInput(const float deltaTime) override final;
	void OnRelease() override final;
	void OnRender() override final;
	void OnMessage(rdt::Message msg) override final;
};