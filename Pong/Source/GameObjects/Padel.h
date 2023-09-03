#pragma once
#include "Pong.h"

class Padel : public Radiant::GameObject {
private:
	Radiant::Rect m_sprite;
	Radiant::Color m_sprite_color;

	Radiant::Translation m_translation;
	double acceleration;

	std::vector<Radiant::InputState> left_cond;
	std::vector<Radiant::InputState> right_cond;
	std::vector<Radiant::InputState> up_cond;
	std::vector<Radiant::InputState> down_cond;


public:
	Padel(double xPos, double yPos);
	~Padel();

	void OnUpdate(const float deltaTIme) override final;
	void OnRender() override final;
	void OnEndFrame() override final;

	void SetSpriteColor(Radiant::Color nColor);
	void SetTransparent(bool transparency);

	void SetLeftControl(const std::vector <Radiant::InputState>& conditions);
	void SetRightControl(const std::vector <Radiant::InputState>& conditions);
	void SetUpControl(const std::vector <Radiant::InputState>& conditions);
	void SetDownControl(const std::vector <Radiant::InputState>& conditions);

	const Radiant::Polygon& GetPolygon() { return m_sprite; }

};