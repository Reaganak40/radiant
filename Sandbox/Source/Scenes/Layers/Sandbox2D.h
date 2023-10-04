#pragma once
#include <Radiant.h>

class Sandbox2D : public rdt::Layer {
private:
public:
	Sandbox2D();
	~Sandbox2D();

	void OnRender() override final;
};