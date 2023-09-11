#include "Game.h"

class Level : public rdt::Scene {
private:
	bool previously_bounded;
	bool loaded_textures;
public:
	Level();
	~Level();

	void OnRegister() override final;
	void OnBind() override final;
	void OnRelease() override final;
	void OnRender() override final;
};