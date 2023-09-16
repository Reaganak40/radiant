#pragma once
#include "Game.h"
#include "GameObjects/PacDot.h"

#define PACMAN_INDEX 0
#define GHOST_INDEX_START 2
#define GHOST_INDEX_END 5
#define HIGHSCORE_INDEX 6
#define _1UP_INDEX 7
#define _1UP_SCORE_INDEX 8
#define HIGHSCORE_VAL_INDEX 9


class Level : public rdt::Scene {
private:
	bool previously_bounded;
	bool loaded_textures;
	bool ghosts_blinking;
	bool waiting_respawn;

	rdt::Timer m_power_timer;
	rdt::Timer m_spawn_timer;
	rdt::Timer m_show_hit_timer;
	rdt::Timer m_1up_timer;

	std::array<std::array<PacDot*, NUM_TILES_X>, NUM_TILES_Y> m_dotMap;
	int playerScore;
	int highScore;

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
	void UpdatePlayerScore(int pointsToAdd);
};