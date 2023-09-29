#include "Game.h"

#define PAC_DOT_WIDTH 5

class PacDot : public rdt::GameObject {
private:
	rdt::Vec2d spawnPos;
	rdt::Color m_color;

	bool m_eaten;
	bool m_power_dot;
	bool m_show;
	rdt::Timer m_blinking_timer;

public:
	PacDot(double xPos, double yPos);
	~PacDot();

	void OnBind() override final;
	void OnRelease() override final;
	void OnProcessInput(const float deltaTIme) override final;
	void OnFinalUpdate() override final;
	void OnRender() override final;

	void SetColor(rdt::Color nColor);

	bool IsEaten();
	void Eat();
	void Reset();
	void MakePowerDot();
	bool IsPowerDot();

	bool ShouldEat(const rdt::Vec2d& position);
};