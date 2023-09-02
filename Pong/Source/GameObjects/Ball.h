#pragma once
#include "Pong.h"

class Ball : public Radiant::GameObject {
private:
	Radiant::Rect m_sprite;
	Radiant::Translation m_translation;
	double acceleration;

public:
	Ball(double xPos, double yPos);
	~Ball();

	void OnUpdate(const float deltaTIme) override final;
	void OnRender() override final;
	void OnEndFrame() override final;

};