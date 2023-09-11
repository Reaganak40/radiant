#pragma once
#include "Pong.h"

class Ball : public rdt::GameObject {
private:
	rdt::Vec2d spawnPos;
	rdt::Color m_sprite_color;
	double acceleration;
	bool m_fill_sprite;

	rdt::Timer m_timer;

	unsigned int m_score[2];
public:
	Ball(double xPos, double yPos);
	~Ball();

	void OnBind() override final;
	void OnProcessInput(const float deltaTIme) override final;
	void OnFinalUpdate() override final;
	void OnRender() override final;

	void Reset();

	friend class Scoreboard;
	friend class Padel;
private:
	void SetUpBall();
	void StartMovingBall();

	void GetScore(unsigned int& player1, unsigned int& player2);
};