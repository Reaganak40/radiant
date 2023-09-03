#include "Pong.h"

class Ball : public Radiant::GameObject {
private:
	Radiant::Rect m_sprite;
	Radiant::Color m_sprite_color;
public:
	Ball(double xPos, double yPos);
	~Ball();

	void OnUpdate(const float deltaTIme) override final;
	void OnRender() override final;
	void OnEndFrame() override final;
};