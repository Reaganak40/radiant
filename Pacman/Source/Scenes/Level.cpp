#include "Level.h"
#include "GameObjects/Pacman.h"
#include "GameObjects/Map.h"
#include "GameObjects/Ghost.h"

using namespace rdt;

Level::Level()
	: previously_bounded(false), loaded_textures(false), m_power_timer(10.0)
{
	for (int i = 0; i < NUM_TILES_Y; i++) {
		m_dotMap[i].fill(nullptr);
	}
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
		Texture& pacmanTex = TextureManager::LoadTextureFromPNG("pacman", "Resources/Textures/pacman.png");
		pacmanTex.DefineTextureAtlas(51, 51, 1, 12, 16);

		TextureManager::LoadTextureFromPNG("map", "Resources/Textures/map.png");

		Texture& ghostTex = TextureManager::LoadTextureFromPNG("ghost", "Resources/Textures/ghost.png");
		ghostTex.DefineTextureAtlas(111, 111, 4, 12, 16);

		TextureManager::LoadTextureFromPNG("powerdot", "Resources/Textures/powerDot.png");
	}

	Pacman* pacman;
	m_game_objects.push_back(pacman = new Pacman(PACMAN_SPAWN_X, PACMAN_SPAWN_Y));
	pacman->RegisterToRealm(m_realms[0]);

	Map* map;
	m_game_objects.push_back(map = new Map);
	map->RegisterToRealm(m_realms[0]);
	pacman->AddMapPtr(map);

	Ghost* blinky;
	m_game_objects.push_back(blinky = new Ghost(BLINKY));
	blinky->RegisterToRealm(m_realms[0]);
	blinky->AddMapPtr(map);

	Ghost* inky;
	m_game_objects.push_back(inky = new Ghost(INKY));
	inky->RegisterToRealm(m_realms[0]);
	inky->AddMapPtr(map);

	Ghost* pinky;
	m_game_objects.push_back(pinky = new Ghost(PINKY));
	pinky->RegisterToRealm(m_realms[0]);
	pinky->AddMapPtr(map);

	Ghost* clyde;
	m_game_objects.push_back(clyde = new Ghost(CLYDE));
	clyde->RegisterToRealm(m_realms[0]);
	clyde->AddMapPtr(map);

	/* Adds dots to the map. */
	for (int row = 0; row < NUM_TILES_Y; row++) {
		for (int col = 0; col < NUM_TILES_X; col++) {

			if (row == 22 && (col == 15 || col == 16)) {
				continue;
			}

			if (row > 7 && row < 19) {
				if (col != 8 && col != 23) {
					continue;
				}
			}

			if (map->IsInMap(row, col)) {
				Vec2d spawnPos = map->GetWorldCoordinates({ col, row });
				PacDot* dot;
				m_game_objects.push_back(dot = new PacDot(spawnPos.x, spawnPos.y));
				dot->RegisterToRealm(m_realms[0]);

				if ((row == 22 || row == 2) && (col == 28 || col == 3)) {
					dot->MakePowerDot();
				}

				m_dotMap[row][col] = dot;
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

void Level::OnProcessInput(const float deltaTime)
{
	if (m_power_timer.IsRunning()) {
		if (m_power_timer.Update(deltaTime)) {
			DeactivatePowerMode();
		}
	}

	RunProcessInputQueue(deltaTime);
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
	Vec2i pacmanCoords = ((Pacman*)m_game_objects[0])->GetMapCoordinates();
	PacDot* dot = m_dotMap[pacmanCoords.y][pacmanCoords.x];
	if (dot != nullptr && !dot->IsEaten()) {
		dot->Eat();

		if (dot->IsPowerDot()) {
			ActivatePowerMode();
		}
	}

	RunRenderQueue();
}

void Level::ActivatePowerMode()
{
	m_power_timer.Start();

	if (m_power_timer.IsRunning()) {

		// ghosts start at index 2
		for (int i = 2; i <= 5; i++) {
			((Ghost*)m_game_objects.at(i))->SetVulnerable(true);
		}
	}
}

void Level::DeactivatePowerMode()
{
	// ghosts start at index 2
	for (int i = 2; i <= 5; i++) {
		((Ghost*)m_game_objects.at(i))->SetVulnerable(false);
	}
}
