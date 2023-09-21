#include "Level.h"
#include "GameObjects/Pacman.h"
#include "GameObjects/Map.h"
#include "GameObjects/Ghost.h"
#include "GameObjects/UI.h"

using namespace rdt;

Level::Level()
	: m_power_timer(10.0), m_spawn_timer(2.5), m_show_hit_timer(0.5), m_1up_timer(0.25)
{
	GState.SetStateCount(LSF_MaxFlags);

	for (int i = 0; i < NUM_TILES_Y; i++) {
		m_dotMap[i].fill(nullptr);
	}

	playerScore = 0;
	highScore = 0;
	levelDotCount = 0;
	lifeCount = 3;
	m_pacman_death_state = PDS_NoDeath;
	RegisterToMessageBus("level");
}

Level::~Level()
{
}

void Level::OnRegister()
{
	if (m_realms.size() == 0) {
		m_realms.push_back(Physics::CreateRealm());
		m_realms.push_back(Physics::CreateRealm());
		m_realms.push_back(Physics::CreateRealm());
	}

	if (!GState.CheckState(LSF_LoadedTextures)) {
		Texture& pacmanTex = TextureManager::LoadTextureFromPNG("pacman", "Resources/Textures/pacman.png");
		pacmanTex.DefineTextureAtlas(52, 52, 4, 3, 16);

		Texture& deathTex = TextureManager::LoadTextureFromPNG("death", "Resources/Textures/death.png");
		deathTex.DefineTextureAtlas(59, 57, 1, 10, 8);

		TextureManager::LoadTextureFromPNG("map", "Resources/Textures/map.png");

		Texture& ghostTex = TextureManager::LoadTextureFromPNG("ghost", "Resources/Textures/ghost.png");
		ghostTex.DefineTextureAtlas(112, 112, 4, 12, 16);

		TextureManager::LoadTextureFromPNG("powerdot", "Resources/Textures/powerDot.png");

		Texture& fontTex = TextureManager::LoadTextureFromPNG("font", "Resources/Textures/font.png");
		fontTex.DefineTextureAtlas(28, 28, 15, 3, 4);
	}

	Pacman* pacman;
	m_game_objects.push_back(pacman = new Pacman(PACMAN_SPAWN_X, PACMAN_SPAWN_Y));
	pacman->RegisterToRealm(m_realms[0]);

	Map* map;
	m_game_objects.push_back(map = new Map);
	map->RegisterToRealm(m_realms[0]);

	Ghost* blinky;
	m_game_objects.push_back(blinky = new Ghost(BLINKY));
	blinky->RegisterToRealm(m_realms[0]);

	Ghost* inky;
	m_game_objects.push_back(inky = new Ghost(INKY));
	inky->RegisterToRealm(m_realms[0]);

	Ghost* pinky;
	m_game_objects.push_back(pinky = new Ghost(PINKY));
	pinky->RegisterToRealm(m_realms[0]);


	Ghost* clyde;
	m_game_objects.push_back(clyde = new Ghost(CLYDE));
	clyde->RegisterToRealm(m_realms[0]);

	UI* highScore;
	m_game_objects.push_back(highScore = new UI(UI_Text, 10, {18, 18}));
	highScore->RegisterToRealm(m_realms[2]);
	highScore->SetOrigin({ SCREEN_WIDTH / 2 - 75, SCREEN_HEIGHT - 20 });
	highScore->SetAlignment(TEXT_LEFT);
	highScore->SetText("HIGH SCORE");

	UI* _1up;
	m_game_objects.push_back(_1up = new UI(UI_Text, 3, { 18, 18 }));
	_1up->RegisterToRealm(m_realms[2]);
	_1up->SetOrigin({ 75, SCREEN_HEIGHT - 20 });
	_1up->SetAlignment(TEXT_LEFT);
	_1up->SetText("1UP");

	UI* _1score;
	m_game_objects.push_back(_1score = new UI(UI_Text, 7, { 18, 18 }));
	_1score->RegisterToRealm(m_realms[2]);
	_1score->SetOrigin({ 25, SCREEN_HEIGHT - 45 });
	_1score->SetAlignment(TEXT_RIGHT);
	_1score->SetText("0");

	UI* highScoreVal;
	m_game_objects.push_back(highScoreVal = new UI(UI_Text, 7, { 18, 18 }));
	highScoreVal->RegisterToRealm(m_realms[2]);
	highScoreVal->SetOrigin({ SCREEN_WIDTH / 2 - 55, SCREEN_HEIGHT - 45 });
	highScoreVal->SetAlignment(TEXT_RIGHT);
	highScoreVal->SetText("");

	UI* lifeDisplay;
	m_game_objects.push_back(lifeDisplay = new UI(UI_Image, 5, { 30, 30 }));
	lifeDisplay->RegisterToRealm(m_realms[2]);
	lifeDisplay->SetOrigin({ 30, 20 });
	lifeDisplay->SetAlignment(TEXT_LEFT);
	lifeDisplay->SetText("LL");

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
	m_1up_timer.Start();

	m_GUIs.push_back(new DiagnosticsGUI);
}

void Level::OnBind()
{
	Renderer::SetBackgroundColor(Color(0.0509f, 0.0509f, 0.0509f, 1.0f));

	if (!GState.CheckState(LSF_PreviouslyBounded)) {
		for (auto& object : m_game_objects) {
			object->OnBind();
		}
		GState.SetState(LSF_PreviouslyBounded, true);
	}

	for (auto& gui : m_GUIs) {
		Renderer::AttachGui(gui);
	}

	Physics::ActivateRealm(m_realms[0]);

	PauseGame();
	m_spawn_timer.Start();

	GState.SetState(LSF_InkyOut, false);
	GState.SetState(LSF_ClydeOut, false);
}

void Level::OnProcessInput(const float deltaTime)
{

	switch (m_pacman_death_state) {
	case PDS_NoDeath:
		break;
	case PDS_ShowHit:
		PacmanDeathShowHitPhase(deltaTime);
		break;
	case PDS_DeathAnimation:
		break;
	case PDS_Repawn:
		Respawn();
		m_pacman_death_state = PDS_NoDeath;
		break;
	}

	if (m_power_timer.IsRunning()) {
		if (m_power_timer.Update(deltaTime)) {
			DeactivatePowerMode();
		}
		else if (m_power_timer.GetTimeLeft() <= 5) {
			if (!GState.CheckState(LSF_GhostsBlinking)) {
				StartBlinking();
			}
		}
	}

	if (m_spawn_timer.IsRunning()) {
		if (m_spawn_timer.Update(deltaTime)) {
			ResumeGame();
		}
	}

	if (m_1up_timer.IsRunning()) {
		if (m_1up_timer.Update(deltaTime)) {
			bool show = ((UI*)m_game_objects.at(_1UP_INDEX))->IsShowing();
			((UI*)m_game_objects.at(_1UP_INDEX))->SetShow(!show);
			m_1up_timer.Start();
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

		UpdatePlayerScore(10);
		levelDotCount++;

		if (levelDotCount >= 30 && !GState.CheckState(LSF_InkyOut)) {
			SendMessage("inky", PMT_LeaveHome, nullptr);
			GState.SetState(LSF_InkyOut, true);
		}
		else if (levelDotCount >= 60 && !GState.CheckState(LSF_ClydeOut)) {
			SendMessage("clyde", PMT_LeaveHome, nullptr);
			GState.SetState(LSF_ClydeOut, true);
		}

	}

	RunRenderQueue();
}

void Level::OnMessage(rdt::Message msg)
{
	switch (msg.type) {
	case PMT_PacmanHit:
		m_pacman_death_state = PDS_ShowHit;
		break;
	case PMT_EndDeathAnimation:
		m_pacman_death_state = PDS_Repawn;
		break;
	}
}

void Level::ActivatePowerMode()
{
	m_power_timer.Start();

	if (m_power_timer.IsRunning()) {
		SendDirectMessage("blinky", PMT_MakeVulnerable);
		SendDirectMessage("inky",   PMT_MakeVulnerable);
		SendDirectMessage("pinky",  PMT_MakeVulnerable);
		SendDirectMessage("clyde",  PMT_MakeVulnerable);
	}

	GState.SetState(LSF_GhostsBlinking, false);
}

void Level::DeactivatePowerMode()
{
	SendDirectMessage("blinky", PMT_StopVulnerability);
	SendDirectMessage("inky",   PMT_StopVulnerability);
	SendDirectMessage("pinky",  PMT_StopVulnerability);
	SendDirectMessage("clyde",  PMT_StopVulnerability);

	GState.SetState(LSF_GhostsBlinking, false);
}

void Level::StartBlinking()
{
	SendDirectMessage("blinky", PMT_StartBlinking);
	SendDirectMessage("inky",   PMT_StartBlinking);
	SendDirectMessage("pinky",  PMT_StartBlinking);
	SendDirectMessage("clyde",  PMT_StartBlinking);

	GState.SetState(LSF_GhostsBlinking, true);
}

void Level::PauseGame()
{
	SendDirectMessage("blinky", PMT_PauseGame);
	SendDirectMessage("inky",   PMT_PauseGame);
	SendDirectMessage("pinky",  PMT_PauseGame);
	SendDirectMessage("clyde",  PMT_PauseGame);
	SendDirectMessage("pacman", PMT_PauseGame);

	Physics::DeactivateRealm(m_realms[0]);
}

void Level::ResumeGame()
{
	SendDirectMessage("blinky", PMT_ResumeGame);
	SendDirectMessage("inky",   PMT_ResumeGame);
	SendDirectMessage("pinky",  PMT_ResumeGame);
	SendDirectMessage("clyde",  PMT_ResumeGame);
	SendDirectMessage("pacman", PMT_ResumeGame);

	Physics::ActivateRealm(m_realms[0]);
}

void Level::Respawn()
{
	SendDirectMessage("pacman", PMT_Respawn);
	SendDirectMessage("pacman", PMT_PauseGame);

	SendDirectMessage("blinky", PMT_Respawn);
	SendDirectMessage("inky",   PMT_Respawn);
	SendDirectMessage("pinky",  PMT_Respawn);
	SendDirectMessage("clyde",  PMT_Respawn);
	SendDirectMessage("pacman", PMT_Respawn);

	m_spawn_timer.SetInterval(1.7f);
	m_spawn_timer.Start();

	GState.SetState(LSF_InkyOut, false);
	GState.SetState(LSF_ClydeOut, false);

	lifeCount--;
	UpdateLifeDisplay();
}

void Level::UpdatePlayerScore(int pointsToAdd)
{
	playerScore += pointsToAdd;
	((UI*)m_game_objects.at(_1UP_SCORE_INDEX))->SetText(std::to_string(playerScore));

	if (playerScore > highScore) {
		highScore = playerScore;
		((UI*)m_game_objects.at(HIGHSCORE_VAL_INDEX))->SetText(std::to_string(highScore));
	}
}

void Level::UpdateLifeDisplay()
{
	std::string txt;
	for (int i = 1; i < lifeCount; i++) {
		txt += "L";
	}
	((UI*)m_game_objects.at(LIFE_DISPLAY_INDEX))->SetText(txt);
}

void Level::PacmanDeathShowHitPhase(const float deltaTime)
{
	if (m_show_hit_timer.IsRunning()) {
		if (m_show_hit_timer.Update(deltaTime)) {
			SendDirectMessage("pacman", PMT_StartDeathAnimation);
			m_pacman_death_state = PDS_DeathAnimation;
		}
	}
	else {
		m_show_hit_timer.Start();
		PauseGame();
		SendDirectMessage("pacman", PMT_ResumeGame);
	}
}

