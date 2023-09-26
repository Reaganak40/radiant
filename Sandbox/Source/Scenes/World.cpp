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
	m_sound = SoundEngine::LoadAudio("Resources/Sounds/30countdown.wav");
}

void World::OnProcessInput(float deltaTime)
{

	if (Input::CheckKeyboardState(std::vector<InputState>{T_KEY_PRESS})) {
		printf("Play sound!\n");
		SoundEngine::PlaySound(m_sound, Vec3f::Zero());
	}

	RunProcessInputQueue(deltaTime);
}

