#include "Game.h"

class Pacman : public Radiant::GameObject {
private:
	Radiant::Vec2d spawnPos;

	std::vector<Radiant::InputState> left_cond;
	std::vector<Radiant::InputState> right_cond;
	std::vector<Radiant::InputState> up_cond;
	std::vector<Radiant::InputState> down_cond;
public:
	Pacman(double xPos, double yPos);
	~Pacman();

	void OnBind() override final;
	void OnRelease() override final;
	void OnProcessInput(const float deltaTIme) override final;
	void OnFinalUpdate() override final;
	void OnRender() override final;
};