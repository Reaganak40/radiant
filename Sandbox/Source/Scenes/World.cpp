#include "World.h"

using namespace rdt;

World::World()
{
}

World::~World()
{
}

void World::OnRegister()
{
	m_sound = SoundManager::LoadAudio("Resources/Sounds/sample3.wav");
}

void World::OnProcessInput(float deltaTime)
{

	if (Input::CheckKeyboardState(std::vector<InputState>{T_KEY_PRESS})) {
		printf("Play sound!\n");
		SoundManager::PlaySound(m_sound);
	}

	RunProcessInputQueue(deltaTime);
}

