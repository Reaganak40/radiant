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
#define LIFE_DISPLAY_INDEX 10
#define READY_INDEX 11
#define GAMEOVER_INDEX1 12
#define GAMEOVER_INDEX2 13


enum LevelStateFlags {
	LSF_PreviouslyBounded,
	LSF_LoadedTextures,
	LSF_GhostsBlinking,
	LSF_InkyOut,
	LSF_ClydeOut,
	LSF_AtLevelStart,
	LSF_GameOver,
	LSF_MaxFlags
};
class Level : public rdt::Scene {
private:

	rdt::Timer m_power_timer;
	rdt::Timer m_spawn_timer;
	rdt::Timer m_show_hit_timer;
	rdt::Timer m_1up_timer;

	std::array<std::array<PacDot*, NUM_TILES_X>, NUM_TILES_Y> m_dotMap;
	int playerScore;
	int m_highScore;
	int levelDotCount;
	int lifeCount;

	PacmanDeathSequence m_pacman_death_state;

public:
	Level();
	~Level();

	void OnRegister() override final;
	void OnBind() override final;
	void OnProcessInput(const float deltaTime) override final;
	void OnFinalUpdate() override final;
	void OnRelease() override final;
	void OnRender() override final;
	void OnMessage(rdt::Message msg) override final;

private:
	void ActivatePowerMode();
	void DeactivatePowerMode();
	void StartBlinking();
	void PauseGame();
	void ResumeGame();
	void Respawn();
	void UpdatePlayerScore(int pointsToAdd);
	void UpdateLifeDisplay();

	void GameOver();

	void PacmanDeathShowHitPhase(const float deltaTime);
};