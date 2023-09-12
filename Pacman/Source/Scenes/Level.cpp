#include "Level.h"
#include "GameObjects/Pacman.h"
#include "GameObjects/Map.h"
#include "GameObjects/PacDot.h"

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
		TextureManager::LoadTextureFromPNG("map", "Resources/Textures/map.png");
	}

	Pacman* pacman;
	m_game_objects.push_back(pacman = new Pacman(PACMAN_SPAWN_X, PACMAN_SPAWN_Y));
	pacman->RegisterToRealm(m_realms[0]);

	Map* map;
	m_game_objects.push_back(map = new Map);
	map->RegisterToRealm(m_realms[0]);
	pacman->AddMapPtr(map);

	for (int row = 0; row < NUM_TILES_Y; row++) {
		for (int col = 0; col < NUM_TILES_X; col++) {

			if (row == 22 && (col == 13 || col == 12)) {
				continue;
			}

			if (row > 7 && row < 19) {
				if (col != 5 && col != 20) {
					continue;
				}
			}

			if (map->IsInMap(row, col)) {
				Vec2d spawnPos = map->GetWorldCoordinates({ col, row });
				PacDot* dot;
				m_game_objects.push_back(dot = new PacDot(spawnPos.x, spawnPos.y));
				dot->RegisterToRealm(m_realms[0]);
			}
		}
	}

	m_GUIs.push_back(new DiagnosticsGUI);
}

void Level::OnBind()
{
	Renderer::SetBackgroundColor(Color(0.0509f, 0.0509f, 0.0509f, 1.0f));

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

	for (auto& gui : m_GUIs) {
		Renderer::DetachGui(gui);
	}
}

void Level::OnRender()
{
	RunRenderQueue();
}
