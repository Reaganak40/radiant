#include "Game.h"

class Level : public Radiant::Scene {
private:
	bool previously_bounded;
public:
	Level();
	~Level();

	void OnRegister() override final;
	void OnBind() override final;
	void OnRelease() override final;
	void OnRender() override final;
};