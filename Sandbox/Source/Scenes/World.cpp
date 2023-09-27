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
	SoundEngine::LoadResource("sample3", "Resources/Sounds/sample3.wav");
	m_sound = SoundEngine::CreateNewSound("sample3", new SoundEffect);
}

void World::OnProcessInput(float deltaTime)
{

	if (Input::CheckKeyboardState(std::vector<InputState>{T_KEY_PRESS})) {
		SoundEngine::PlaySound(m_sound, Vec3f::Zero(), true);
	}

	RunProcessInputQueue(deltaTime);
}

