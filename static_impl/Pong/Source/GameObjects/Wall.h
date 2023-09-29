#include "Pong.h"

class Wall : public rdt::GameObject {
private:
	rdt::Vec2d spawnPos;
	bool m_is_visible;

public:
	Wall(double xPos, double yPos);
	~Wall();

	void OnBind() override final;
	void OnProcessInput(const float deltaTIme) override final;
	void OnFinalUpdate() override final;
	void OnRender() override final;

	void SetWallVisibility(bool isVisible);
};