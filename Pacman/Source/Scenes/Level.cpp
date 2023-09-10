#include "Level.h"
#include "GameObjects/Pacman.h"

using namespace Radiant;

Level::Level()
	: previously_bounded(false)
{
}

Level::~Level()
{
}

void Level::OnRegister()
{
	if (m_realms.size() == 0) {
		m_realms.push_back(Physics::CreateRealm());
	}

	Pacman* pacman;
	m_game_objects.push_back(pacman = new Pacman(100, 100));
	pacman->RegisterToRealm(m_realms[0]);
}

void Level::OnBind()
{
	Renderer::SetBackgroundColor(BLACK);

	if (!previously_bounded) {
		for (auto& object : m_game_objects) {
			object->OnBind();
		}
		previously_bounded = true;
	}

	for (auto& realm : m_realms) {
		Physics::ActivateRealm(realm);
	}
}

void Level::OnRelease()
{
	for (auto& realm : m_realms) {
		Physics::DeactivateRealm(realm);
	}
}

void Level::OnRender()
{
	RunRenderQueue();
}
