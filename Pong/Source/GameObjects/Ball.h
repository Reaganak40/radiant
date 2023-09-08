#pragma once
#include "Pong.h"

class Ball : public Radiant::GameObject {
private:
	Radiant::Vec2d spawnPos;
	Radiant::Color m_sprite_color;
	double acceleration;
	bool m_fill_sprite;

	Radiant::Timer m_timer;

	unsigned int m_score[2];
public:
	Ball(double xPos, double yPos);
	~Ball();

	void OnRegister() override final;
	void OnProcessInput(const float deltaTIme) override final;
	void OnFinalUpdate() override final;
	void OnRender() override final;

	friend class Scoreboard;
private:
	void SetUpBall();
	void StartMovingBall();

	void GetScore(unsigned int& player1, unsigned int& player2);
};