#pragma once
#include "Pong.h"

class Padel : public Radiant::GameObject {
private:
	Radiant::Vec2d spawnPos;
	Radiant::Color m_sprite_color;

	std::vector<Radiant::InputState> left_cond;
	std::vector<Radiant::InputState> right_cond;
	std::vector<Radiant::InputState> up_cond;
	std::vector<Radiant::InputState> down_cond;
public:
	Padel(double xPos, double yPos);
	~Padel();

	void OnRegister() override final;
	void OnProcessInput(const float deltaTIme) override final;
	void OnFinalUpdate() override final;
	void OnRender() override final;

	void SetSpriteColor(Radiant::Color nColor);
	void SetTransparent(bool transparency);

	void SetLeftControl(const std::vector <Radiant::InputState>& conditions);
	void SetRightControl(const std::vector <Radiant::InputState>& conditions);
	void SetUpControl(const std::vector <Radiant::InputState>& conditions);
	void SetDownControl(const std::vector <Radiant::InputState>& conditions);

};