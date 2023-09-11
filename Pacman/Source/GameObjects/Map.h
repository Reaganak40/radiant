#include "Game.h"

#define MAP_WIDTH 600
#define MAP_HEIGHT 664
#define MAP_LAYER 0

class Map : public rdt::GameObject {
private:
public:
	Map();
	~Map();

	void OnBind() override final;
	void OnRelease() override final;
	void OnProcessInput(const float deltaTIme) override final;
	void OnFinalUpdate() override final;
	void OnRender() override final;
};