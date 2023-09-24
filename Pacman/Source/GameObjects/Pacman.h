#pragma once
#include "Game.h"
#include "Map.h"


enum PacmanGameState {
	PGS_Paused,
	PGS_Spawned,
	PGS_IsHit,
	PGS_InRespawn,
	PGS_InDeathAnimation,
	PGS_IsGameOver,
	PGS_IsEndLevel,
	PGS_ShowEatenGhost,
	PGS_MaxState
};

class Pacman : public rdt::GameObject {
private:
	rdt::Vec2d spawnPos;

	Map* m_map;

	std::vector<rdt::InputState> left_cond;
	std::vector<rdt::InputState> right_cond;
	std::vector<rdt::InputState> up_cond;
	std::vector<rdt::InputState> down_cond;

	rdt::Timer m_texture_timer;
	rdt::Timer m_death_timer;

	rdt::Vec2i m_target_coords;
	PacmanMoveDirection m_direction;

	int current_frame;
	int df;
	int m_frame_row;
	int m_frame_col;

public:
	Pacman(double xPos, double yPos);
	~Pacman();

	void OnBind() override final;
	void OnRelease() override final;
	void OnProcessInput(const float deltaTime) override final;
	void OnFinalUpdate() override final;
	void OnRender() override final;
	void OnMessage(rdt::Message msg) override final;


	rdt::Vec2i GetMapCoordinates();
	rdt::Vec2d GetWorldCoordinates();
	PacmanMoveDirection GetDirection();


	void BeginDeathAnimation();
	bool InRespawn();
	bool IsHit();

private:
	void Respawn();
	void OnEndLevel();
	void OnNewLevel();
	void OnShowEaten(bool showing);
	void SetPause(bool pause);
	void AddGameObjectPtr(rdt::MessageID from, rdt::GameObjectPtrData* data);
	void UpdateVelocityAndDirection();
	void UpdateTextureFrame(const float deltaTime);
	void ReAlignToMap();
	void OnHit();
	void GetTargetInputDirection(bool* input4, float* inputTimestamps4);

};