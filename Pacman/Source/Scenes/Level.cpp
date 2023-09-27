#include "Level.h"
#include "GameObjects/Map.h"
#include "GameObjects/Ghost.h"
#include "GameObjects/UI.h"
#include "GameObjects/Points.h"

using namespace rdt;

Level::Level()
	: m_power_timer(10.0), m_spawn_timer(5.0f), m_show_hit_timer(0.5), m_1up_timer(0.25), m_end_level_timer(1.0f),
	m_show_eaten_timer(1.0f)
{
	GState.SetStateCount(LSF_MaxFlags);

	for (int i = 0; i < NUM_TILES_Y; i++) {
		m_dotMap[i].fill(nullptr);
	}

	playerScore = 0;
	m_highScore = 28180;
	levelDotCount = 0;
	lifeCount = 4;
	levelCount = 1;
	ghostEatenCount = 0;
	m_pacman_death_state = PDS_NoDeath;
	RegisterToMessageBus("level");

	devToolsEnabled = true;
	m_pacman_ptr = nullptr;
	m_startLevelSound = 0;
	m_currChomp = 0;
	m_currSiren = 0;
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

	if (!GState.CheckState(LSF_LoadedResources)) {
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

		Texture& fruitTex = TextureManager::LoadTextureFromPNG("fruit", "Resources/Textures/fruit.png");
		fruitTex.DefineTextureAtlas(24, 28, 1, 8, 8);

		Texture& pointsTex = TextureManager::LoadTextureFromPNG("points", "Resources/Textures/points.png");
		pointsTex.DefineTextureAtlas(64, 28, 8, 2, 8);

		SoundEngine::LoadResource("startLevel", "Resources/Sounds/pacman_beginning.wav");
		m_startLevelSound = SoundEngine::CreateNewSound("startLevel", new SoundEffect);

		SoundEngine::LoadResource("chomp1", "Resources/Sounds/munch_1.wav");
		SoundEngine::LoadResource("chomp2", "Resources/Sounds/munch_2.wav");
		m_chompSound[0] = SoundEngine::CreateNewSound("chomp1", new SoundEffect);
		m_chompSound[1] = SoundEngine::CreateNewSound("chomp2", new SoundEffect);

		SoundEngine::LoadResource("siren1", "Resources/Sounds/siren_1.wav");
		SoundEngine::LoadResource("siren2", "Resources/Sounds/siren_2.wav");
		SoundEngine::LoadResource("siren3", "Resources/Sounds/siren_3.wav");
		SoundEngine::LoadResource("siren4", "Resources/Sounds/siren_4.wav");
		SoundEngine::LoadResource("siren5", "Resources/Sounds/siren_5.wav");
		m_sirenSound[0] = SoundEngine::CreateNewSound("siren1", new SoundEffect);
		m_sirenSound[1] = SoundEngine::CreateNewSound("siren2", new SoundEffect);
		m_sirenSound[2] = SoundEngine::CreateNewSound("siren3", new SoundEffect);
		m_sirenSound[3] = SoundEngine::CreateNewSound("siren4", new SoundEffect);
		m_sirenSound[4] = SoundEngine::CreateNewSound("siren5", new SoundEffect);
	}

	m_game_objects.push_back(m_pacman_ptr = new Pacman(PACMAN_SPAWN_X, PACMAN_SPAWN_Y));
	m_pacman_ptr->RegisterToRealm(m_realms[0]);

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

	UI* highScoreText;
	m_game_objects.push_back(highScoreText = new UI(UI_Text, 10, {18, 18}));
	highScoreText->RegisterToRealm(m_realms[2]);
	highScoreText->SetOrigin({ SCREEN_WIDTH / 2 - 75, SCREEN_HEIGHT - 20 });
	highScoreText->SetAlignment(TEXT_LEFT);
	highScoreText->SetText("HIGH SCORE");

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
	highScoreVal->SetText(std::to_string(m_highScore));

	UI* lifeDisplay;
	m_game_objects.push_back(lifeDisplay = new UI(UI_Image, 5, { 30, 30 }));
	lifeDisplay->RegisterToRealm(m_realms[2]);
	lifeDisplay->SetOrigin({ 30, 20 });
	lifeDisplay->SetAlignment(TEXT_LEFT);
	lifeDisplay->SetText("LLL");
	
	UI* readyText;
	m_game_objects.push_back(readyText = new UI(UI_Text, 6, { 19, 19 }));
	readyText->RegisterToRealm(m_realms[2]);
	readyText->SetOrigin({ SCREEN_WIDTH / 2 - 45, 325 });
	readyText->SetAlignment(TEXT_LEFT);
	readyText->SetText("READY!");
	readyText->SetTextColor(YELLOW);

	UI* gameOverText1;
	m_game_objects.push_back(gameOverText1 = new UI(UI_Text, 4, { 19, 19 }));
	gameOverText1->RegisterToRealm(m_realms[2]);
	gameOverText1->SetOrigin({ SCREEN_WIDTH / 2 - 85, 325 });
	gameOverText1->SetAlignment(TEXT_LEFT);
	gameOverText1->SetText("GAME");
	gameOverText1->SetTextColor(Color(1.0f, 0.0f, 0.0f, 1.0f));
	gameOverText1->SetShow(false);

	UI* gameOverText2;
	m_game_objects.push_back(gameOverText2 = new UI(UI_Text, 4, { 19, 19 }));
	gameOverText2->RegisterToRealm(m_realms[2]);
	gameOverText2->SetOrigin({ SCREEN_WIDTH / 2 + 25, 325 });
	gameOverText2->SetAlignment(TEXT_LEFT);
	gameOverText2->SetText("OVER");
	gameOverText2->SetTextColor(Color(1.0f, 0.0f, 0.0f, 1.0f));
	gameOverText2->SetShow(false);

	Fruit* fruit;
	m_game_objects.push_back(fruit = new Fruit);
	fruit->RegisterToRealm(m_realms[0]);
	fruit->SetType(CHERRY);

	Points* points;
	m_game_objects.push_back(points = new Points);
	points->RegisterToRealm(m_realms[0]);

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

	GState.SetState(LSF_AtLevelStart, true);
	GState.SetState(LSF_StartOfGame, true);
	SoundEngine::PlaySound(m_startLevelSound);
	PauseGame();
	m_spawn_timer.Start();

	GState.SetState(LSF_InkyOut, false);
	GState.SetState(LSF_ClydeOut, false);
}

void Level::OnProcessInput(const float deltaTime)
{
	OnDevTools();

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

	if (GState.CheckState(LSF_GameOver)) {
		return;
	}

	if (GState.CheckState(LSF_InEndAnimation)) {
		if (m_end_level_timer.IsRunning()) {
			if (m_end_level_timer.Update(deltaTime)) {
				StartNextLevel();
			}
		}
	}

	if (m_show_eaten_timer.IsRunning()) {
		if (m_show_eaten_timer.Update(deltaTime)) {
			StopShowingEatenGhost();
		}
	} else if (m_power_timer.IsRunning()) {
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
			SoundEngine::PlaySound(m_sirenSound[m_currSiren], Vec3f::Zero(), true);

			if (GState.CheckState(LSF_StartOfGame)) {
				m_spawn_timer.SetInterval(2.5f);
				GState.SetState(LSF_StartOfGame, false);
			}
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

void Level::OnFinalUpdate()
{
	if (GState.CheckState(LSF_GameOver)) {
		return;
	}

	Vec2i pacmanCoords = ((Pacman*)m_game_objects[0])->GetMapCoordinates();
	PacDot* dot = m_dotMap[pacmanCoords.y][pacmanCoords.x];
	if (dot != nullptr && !dot->IsEaten() && dot->ShouldEat(((Pacman*)m_game_objects[0])->GetWorldCoordinates())) {
		OnEat(dot);
	}

	RunFinalUpdateQueue();
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

void Level::OnMessage(rdt::Message msg)
{
	switch (msg.type) {
	case PMT_PacmanHit:
		m_pacman_death_state = PDS_ShowHit;
		SoundEngine::StopSound(m_sirenSound[m_currSiren]);
		break;
	case PMT_EndDeathAnimation:
		m_pacman_death_state = PDS_Repawn;
		break;
	case PMT_StartEndLevelAnimation:
		StartEndLevelAnimation();
		break;
	case PMT_FruitEaten:
		OnFruitEaten((FruitData*)msg.data);
		break;
	case PMT_GhostEaten:
		OnGhostEaten();
		break;
	}
}

void Level::ActivatePowerMode()
{
	if (levelCount >= 19) {
		return;
	}

	m_power_timer.Start();

	if (m_power_timer.IsRunning()) {
		SendDirectMessage("blinky", PMT_MakeVulnerable);
		SendDirectMessage("inky",   PMT_MakeVulnerable);
		SendDirectMessage("pinky",  PMT_MakeVulnerable);
		SendDirectMessage("clyde",  PMT_MakeVulnerable);
	}

	GState.SetState(LSF_GhostsBlinking, false);
	ghostEatenCount = 0;
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

	if (GState.CheckState(LSF_AtLevelStart)) {
		((UI*)m_game_objects[READY_INDEX])->SetShow(true);
	}

	Physics::DeactivateRealm(m_realms[0]);
}

void Level::ResumeGame()
{
	SendDirectMessage("blinky", PMT_ResumeGame);
	SendDirectMessage("inky",   PMT_ResumeGame);
	SendDirectMessage("pinky",  PMT_ResumeGame);
	SendDirectMessage("clyde",  PMT_ResumeGame);
	SendDirectMessage("pacman", PMT_ResumeGame);

	if (GState.CheckState(LSF_AtLevelStart)) {
		((UI*)m_game_objects[READY_INDEX])->SetShow(false);
		GState.SetState(LSF_AtLevelStart, false);
	}

	Physics::ActivateRealm(m_realms[0]);
}

void Level::Respawn()
{
	
	lifeCount--;
	UpdateLifeDisplay();

	if (lifeCount == 0) {
		GameOver();
		return;
	}

	SendDirectMessage("pacman", PMT_Respawn);
	SendDirectMessage("pacman", PMT_PauseGame);

	SendDirectMessage("blinky", PMT_Respawn);
	SendDirectMessage("inky",   PMT_Respawn);
	SendDirectMessage("pinky",  PMT_Respawn);
	SendDirectMessage("clyde",  PMT_Respawn);
	SendDirectMessage("pacman", PMT_Respawn);

	SendDirectMessage("fruit", PMT_Respawn);

	m_spawn_timer.SetInterval(1.7f);
	m_spawn_timer.Start();

	GState.SetState(LSF_InkyOut, false);
	GState.SetState(LSF_ClydeOut, false);
}

void Level::UpdatePlayerScore(int pointsToAdd)
{
	playerScore += pointsToAdd;
	((UI*)m_game_objects.at(_1UP_SCORE_INDEX))->SetText(std::to_string(playerScore));

	if (playerScore > m_highScore) {
		m_highScore = playerScore;
		((UI*)m_game_objects.at(HIGHSCORE_VAL_INDEX))->SetText(std::to_string(m_highScore));
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

void Level::GameOver()
{
	SendMessage("pacman", PMT_GameOver);
	SendMessage("blinky", PMT_GameOver);
	SendMessage("inky",   PMT_GameOver);
	SendMessage("pinky",  PMT_GameOver);
	SendMessage("clyde",  PMT_GameOver);
	GState.SetState(LSF_GameOver, true);

	((UI*)m_game_objects.at(GAMEOVER_INDEX1))->SetShow(true);
	((UI*)m_game_objects.at(GAMEOVER_INDEX2))->SetShow(true);
}

void Level::OnEndLevel()
{
	SendMessage("pacman", PMT_LevelEnded);
	SendMessage("blinky", PMT_LevelEnded);
	SendMessage("inky",   PMT_LevelEnded);
	SendMessage("pinky",  PMT_LevelEnded);
	SendMessage("clyde",  PMT_LevelEnded);
	GState.SetState(LSF_LevelEnded, true);
	Physics::DeactivateRealm(m_realms[0]);

	SoundEngine::StopSound(m_sirenSound[m_currSiren]);
}

void Level::OnEat(PacDot* dot)
{
	dot->Eat();

	SoundEngine::PlaySound(m_chompSound[m_currChomp], Vec3f::Zero(), false);
	m_currChomp = m_currChomp == 0 ? 1 : 0;

	if (dot->IsPowerDot()) {
		ActivatePowerMode();
		UpdatePlayerScore(50);
	}
	else {
		UpdatePlayerScore(10);
	}
	levelDotCount++;

	if (levelDotCount >= 30 && !GState.CheckState(LSF_InkyOut)) {
		SendMessage("inky", PMT_LeaveHome, nullptr);
		GState.SetState(LSF_InkyOut, true);
	}
	else if (levelDotCount >= 60 && !GState.CheckState(LSF_ClydeOut)) {
		SendMessage("clyde", PMT_LeaveHome, nullptr);
		GState.SetState(LSF_ClydeOut, true);
	}

	if (levelDotCount == 70 || levelDotCount == 170) {
		SendMessage("fruit", PMT_ShowFruit);
	}

	if (levelDotCount == DOTS_PER_LEVEL) {
		OnEndLevel();
	}
}

void Level::StartEndLevelAnimation()
{
	m_end_level_timer.Start();
	GState.SetState(LSF_LevelEnded, false);
	GState.SetState(LSF_InEndAnimation, true);
}

void Level::StartNextLevel()
{
	levelCount++;

	// notify end of level animation
	GState.SetState(LSF_InEndAnimation, false);
	SendDirectMessage("pacman", PMT_StartNewLevel);
	SendDirectMessage("blinky", PMT_StartNewLevel);
	SendDirectMessage("inky",   PMT_StartNewLevel);
	SendDirectMessage("pinky",  PMT_StartNewLevel);
	SendDirectMessage("clyde",  PMT_StartNewLevel);

	SendDirectMessage("fruit",  PMT_StartNewLevel);
	SendDirectMessage("fruit", PMT_SetFruit, new FruitData(GetNextFruit()));

	// Run respawn procedure
	SendDirectMessage("pacman", PMT_Respawn);
	SendDirectMessage("blinky", PMT_Respawn);
	SendDirectMessage("inky",   PMT_Respawn);
	SendDirectMessage("pinky",  PMT_Respawn);
	SendDirectMessage("clyde",  PMT_Respawn);
	SendDirectMessage("pacman", PMT_Respawn);

	PauseGame();
	m_spawn_timer.SetInterval(1.7f);
	m_spawn_timer.Start();

	GState.SetState(LSF_InkyOut, false);
	GState.SetState(LSF_ClydeOut, false);

	GState.SetState(LSF_AtLevelStart, true);
	((UI*)m_game_objects[READY_INDEX])->SetShow(true);

	levelDotCount = 0;
	for (int i = 0; i < NUM_TILES_Y; i++) {
		for (int j = 0; j < NUM_TILES_X; j++) {
			if (m_dotMap[i][j] != nullptr) {
				m_dotMap[i][j]->Reset();
			}
		}
	}

	SetVulnerableTimer();
}

void Level::OnFruitEaten(FruitData* fruitData)
{
	UpdatePlayerScore(fruitData->GetValue());
	SendMessage("points", PMT_ShowPointsEarned, new PointData(fruitData->GetValue()));
}

void Level::OnGhostEaten()
{
	unsigned int scored = (2 << ghostEatenCount) * 100;
	UpdatePlayerScore(scored);
	ghostEatenCount++;

	SendMessage("pacman", PMT_ShowEatenGhost);
	SendMessage("blinky", PMT_ShowEatenGhost);
	SendMessage("pinky",  PMT_ShowEatenGhost);
	SendMessage("inky",   PMT_ShowEatenGhost);
	SendMessage("clyde",  PMT_ShowEatenGhost);
	m_show_eaten_timer.Start();

	Vec2d showPos = m_pacman_ptr->GetWorldCoordinates();
	showPos.x -= 2;
	SendMessage("points", PMT_ShowPointsEarned, new PointData(scored, showPos));
}

void Level::StopShowingEatenGhost()
{
	SendMessage("pacman", PMT_StopShowingEatenGhost);
	SendMessage("blinky", PMT_StopShowingEatenGhost);
	SendMessage("pinky",  PMT_StopShowingEatenGhost);
	SendMessage("inky",   PMT_StopShowingEatenGhost);
	SendMessage("clyde",  PMT_StopShowingEatenGhost);
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

void Level::SetVulnerableTimer()
{
	if (levelCount == 2) {
		m_power_timer.SetInterval(9.0f);
	}
	else if (levelCount == 3) {
		m_power_timer.SetInterval(8.0f);
	}
	else if (levelCount == 4 || levelCount == 5) {
		m_power_timer.SetInterval(7.0f);
	}
	else if (levelCount == 6 || levelCount == 7) {
		m_power_timer.SetInterval(6.0f);
	}
	else if (levelCount == 8 || levelCount == 9) {
		m_power_timer.SetInterval(5.0f);
	}
	else if (levelCount >= 10 && levelCount <= 13) {
		m_power_timer.SetInterval(4.0f);
	}
	else if (levelCount >= 14 && levelCount <= 17) {
		m_power_timer.SetInterval(2.0f);
	}
	else {
		m_power_timer.SetInterval(1.0f);
	}
}

void Level::OnDevTools()
{
	if (!devToolsEnabled) {
		return;
	}

	if (Input::CheckKeyboardState(std::vector<InputState>{L_KEY_PRESS})) {
		lifeCount++;
		UpdateLifeDisplay();
	}
	if (Input::CheckKeyboardState(std::vector<InputState>{P_KEY_PRESS})) {
		OnEndLevel();
	}
}

FRUIT_TYPE Level::GetNextFruit()
{
	if (levelCount <= 4) {
		switch (levelCount) {
		case 1:
			return CHERRY;
		case 2:
			return STRAWBERRY;
		case 3:
			return PEACH;
		case 4:
			return PEACH;
		}
	}
	else if (levelCount == 5 || levelCount == 6) {
		return APPLE;
	}
	else if (levelCount == 7 || levelCount == 8) {
		return MELON;
	}
	else if (levelCount == 9 || levelCount == 10) {
		return GALAXIAN;
	}
	else if (levelCount == 11 || levelCount == 12) {
		return BELL;
	}
	
	return KEY;
}

