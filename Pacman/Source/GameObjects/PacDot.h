#include "Game.h"

#define PAC_DOT_WIDTH 5
#define PAC_DOT_LAYER 1

class PacDot : public rdt::GameObject {
private:
	rdt::Vec2d spawnPos;
	rdt::Color m_color;

	bool m_eaten;

public:
	PacDot(double xPos, double yPos);
	~PacDot();

	void OnBind() override final;
	void OnRelease() override final;
	void OnProcessInput(const float deltaTIme) override final;
	void OnFinalUpdate() override final;
	void OnRender() override final;

	void SetColor(rdt::Color nColor);

	void Eat();
	void Reset();
};