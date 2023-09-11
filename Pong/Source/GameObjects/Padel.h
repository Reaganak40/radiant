#pragma once
#include "Pong.h"
#include "Ball.h"

class Padel : public rdt::GameObject {
private:
	rdt::Vec2d spawnPos;
	rdt::Color m_sprite_color;
	double acceleration;
	bool m_fill_sprite;

	std::vector<rdt::InputState> left_cond;
	std::vector<rdt::InputState> right_cond;
	std::vector<rdt::InputState> up_cond;
	std::vector<rdt::InputState> down_cond;

	bool m_AI;
	Ball* m_ball;

public:
	Padel(double xPos, double yPos);
	~Padel();

	void OnBind() override final;
	void OnRelease() override final;
	void OnProcessInput(const float deltaTIme) override final;
	void OnFinalUpdate() override final;
	void OnRender() override final;

	void SetSpriteColor(rdt::Color nColor);
	void SetTransparent(bool transparency);

	void FillSprite(bool fill);

	void SetLeftControl(const std::vector <rdt::InputState>& conditions);
	void SetRightControl(const std::vector <rdt::InputState>& conditions);
	void SetUpControl(const std::vector <rdt::InputState>& conditions);
	void SetDownControl(const std::vector <rdt::InputState>& conditions);

	rdt::Vec2d GetPosition();

	void SetComputerControl(bool isComputer);
	void AddBallReference(Ball* ball);
	
private:
	void OnAIUpdate(const float deltaTime);
};