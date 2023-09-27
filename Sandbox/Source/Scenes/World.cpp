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
	SoundEngine::LoadResource("pacman_beginning", "Resources/Sounds/pacman_beginning.wav");

	m_sound1 = SoundEngine::CreateNewSound("sample3", new SoundEffect);
	m_sound2 = SoundEngine::CreateNewSound("pacman_beginning", new SoundEffect);
}

void World::OnProcessInput(float deltaTime)
{

	if (Input::CheckKeyboardState(std::vector<InputState>{T_KEY_PRESS})) {
		SoundEngine::PlaySound(m_sound1, Vec3f::Zero(), true);
	} else if (Input::CheckKeyboardState(std::vector<InputState>{Y_KEY_PRESS})) {
		SoundEngine::StopSound(m_sound1);
	}

	if (Input::CheckKeyboardState(std::vector<InputState>{O_KEY_PRESS})) {
		SoundEngine::PlaySound(m_sound2, Vec3f::Zero(), false);
	}
	else if (Input::CheckKeyboardState(std::vector<InputState>{P_KEY_PRESS})) {
		SoundEngine::StopSound(m_sound2);
	}

	for (auto& msg : MessageBus::GetBroadcast("SoundEngine")) {
		switch (msg.type) {
		case MT_SoundStopped:
			printf("Sound stopped\n");
			break;
		}
	}

	RunProcessInputQueue(deltaTime);
}

