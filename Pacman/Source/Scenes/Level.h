#pragma once
#include "Game.h"
#include "GameObjects/PacDot.h"

class Level : public rdt::Scene {
private:
	bool previously_bounded;
	bool loaded_textures;
	bool ghosts_blinking;
	bool waiting_respawn;

	rdt::Timer m_power_timer;
	rdt::Timer m_spawn_timer;
	rdt::Timer m_show_hit_timer;

	std::array<std::array<PacDot*, NUM_TILES_X>, NUM_TILES_Y> m_dotMap;
public:
	Level();
	~Level();

	void OnRegister() override final;
	void OnBind() override final;
	void OnProcessInput(const float deltaTime) override final;
	void OnRelease() override final;
	void OnRender() override final;

private:
	void ActivatePowerMode();
	void DeactivatePowerMode();
	void StartBlinking();
	void PauseGame();
	void ResumeGame();
	void Respawn();
};