#pragma once
#include "Game.h"
#include "GameObjects/PacDot.h"

class Level : public rdt::Scene {
private:
	bool previously_bounded;
	bool loaded_textures;

	std::array<std::array<PacDot*, NUM_TILES_X>, NUM_TILES_Y> m_dotMap;
public:
	Level();
	~Level();

	void OnRegister() override final;
	void OnBind() override final;
	void OnRelease() override final;
	void OnRender() override final;
};