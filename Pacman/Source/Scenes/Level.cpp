#include "Level.h"
#include "GameObjects/Pacman.h"

using namespace rdt;

Level::Level()
	: previously_bounded(false), loaded_textures(false)
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

	if (!loaded_textures) {
		TextureManager::LoadTextureFromPNG("pacman", "Resources/Textures/pacman.png");
	}

	Pacman* pacman;
	m_game_objects.push_back(pacman = new Pacman(100, 100));
	pacman->RegisterToRealm(m_realms[0]);

	m_GUIs.push_back(new DiagnosticsGUI);
}

void Level::OnBind()
{
	Renderer::SetBackgroundColor(Color(0.1f, 0.1f, 0.1f, 1.0f));

	if (!previously_bounded) {
		for (auto& object : m_game_objects) {
			object->OnBind();
		}
		previously_bounded = true;
	}

	for (auto& gui : m_GUIs) {
		Renderer::AttachGui(gui);
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
