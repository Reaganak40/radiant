#pragma once
#include "Game.h"
#include "Map.h"




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

	bool m_paused;
	bool m_spawned;
	bool m_is_hit;
	bool m_in_respawn;
	bool m_dead_animation;

public:
	Pacman(double xPos, double yPos);
	~Pacman();

	void OnBind() override final;
	void OnRelease() override final;
	void OnProcessInput(const float deltaTime) override final;
	void OnFinalUpdate() override final;
	void OnRender() override final;
	void OnMessage(rdt::Message msg) override final;

	void Respawn();

	rdt::Vec2i GetMapCoordinates();
	rdt::Vec2d GetWorldCoordinates();
	PacmanMoveDirection GetDirection();

	void SetPause(bool pause);

	void BeginDeathAnimation();
	bool InRespawn();
	bool IsHit();

private:
	void AddGameObjectPtr(rdt::MessageID from, rdt::GameObjectPtrData* data);
	void UpdateVelocityAndDirection();
	void UpdateTextureFrame(const float deltaTime);
	void ReAlignToMap();
	void OnHit();

};