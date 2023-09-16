#include "Level.h"
#include "GameObjects/Pacman.h"
#include "GameObjects/Map.h"
#include "GameObjects/Ghost.h"

using namespace rdt;

Level::Level()
	: previously_bounded(false), loaded_textures(false), m_power_timer(10.0), ghosts_blinking(false),
	m_spawn_timer(2.5), m_show_hit_timer(0.5), waiting_respawn(false)
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
		m_realms.push_back(Physics::CreateRealm());
	}

	if (!loaded_textures) {
		Texture& pacmanTex = TextureManager::LoadTextureFromPNG("pacman", "Resources/Textures/pacman.png");
		pacmanTex.DefineTextureAtlas(51, 51, 4, 3, 16);

		Texture& deathTex = TextureManager::LoadTextureFromPNG("death", "Resources/Textures/death.png");
		deathTex.DefineTextureAtlas(59, 57, 1, 10, 8);

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
	blinky->SetPacmanPtr(pacman);

	Ghost* inky;
	m_game_objects.push_back(inky = new Ghost(INKY));
	inky->RegisterToRealm(m_realms[0]);
	inky->AddMapPtr(map);
	inky->SetPacmanPtr(pacman);
	inky->AddBlinkyPtr(blinky);


	Ghost* pinky;
	m_game_objects.push_back(pinky = new Ghost(PINKY));
	pinky->RegisterToRealm(m_realms[0]);
	pinky->AddMapPtr(map);
	pinky->SetPacmanPtr(pacman);


	Ghost* clyde;
	m_game_objects.push_back(clyde = new Ghost(CLYDE));
	clyde->RegisterToRealm(m_realms[0]);
	clyde->AddMapPtr(map);
	clyde->SetPacmanPtr(pacman);

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
				dot->RegisterToRealm(m_realms[1]);

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

	Physics::ActivateRealm(m_realms[0]);

	PauseGame();
	m_spawn_timer.Start();
}

void Level::OnProcessInput(const float deltaTime)
{

	if (((Pacman*)m_game_objects[0])->IsHit()) {

		if (m_show_hit_timer.IsRunning()) {
			if (m_show_hit_timer.Update(deltaTime)) {
				((Pacman*)m_game_objects[0])->BeginDeathAnimation();
				((Pacman*)m_game_objects[0])->SetPause(false);
				waiting_respawn = true;
			}
		}
		else if (!((Pacman*)m_game_objects[0])->InDeathAnimation()) {
			PauseGame();
			m_show_hit_timer.Start();
		}
	}
	else if (waiting_respawn) {
		Respawn();
	}

	if (m_power_timer.IsRunning()) {
		if (m_power_timer.Update(deltaTime)) {
			DeactivatePowerMode();
		}
		else if (m_power_timer.GetTimeLeft() <= 5) {
			if (!ghosts_blinking) {
				StartBlinking();
			}
		}
	}

	if (m_spawn_timer.IsRunning()) {
		if (m_spawn_timer.Update(deltaTime)) {
			ResumeGame();
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
	if (dot != nullptr && !dot->IsEaten() && dot->ShouldEat(((Pacman*)m_game_objects[0])->GetWorldCoordinates())) {
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

	ghosts_blinking = false;
}

void Level::DeactivatePowerMode()
{
	// ghosts start at index 2
	for (int i = 2; i <= 5; i++) {
		((Ghost*)m_game_objects.at(i))->SetVulnerable(false);
	}

	ghosts_blinking = false;
}

void Level::StartBlinking()
{
	// ghosts start at index 2
	for (int i = 2; i <= 5; i++) {
		((Ghost*)m_game_objects.at(i))->SetIsBlinking(true);
	}
	ghosts_blinking = true;
}

void Level::PauseGame()
{
	// ghosts start at index 2
	for (int i = 2; i <= 5; i++) {
		((Ghost*)m_game_objects.at(i))->SetPause(true);
	}
	((Pacman*)m_game_objects[0])->SetPause(true);

	Physics::DeactivateRealm(m_realms[0]);

}

void Level::ResumeGame()
{
	// ghosts start at index 2
	for (int i = 2; i <= 5; i++) {
		((Ghost*)m_game_objects.at(i))->SetPause(false);
	}
	((Pacman*)m_game_objects[0])->SetPause(false);

	Physics::ActivateRealm(m_realms[0]);

}

void Level::Respawn()
{
	waiting_respawn = false;
	((Pacman*)m_game_objects[0])->Respawn();
	((Pacman*)m_game_objects[0])->SetPause(true);

	for (int i = 2; i <= 5; i++) {
		((Ghost*)m_game_objects.at(i))->Respawn();
	}

	m_spawn_timer.SetInterval(1.7);
	m_spawn_timer.Start();
}
