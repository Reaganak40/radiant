#include "Menu.h"
#include "GameObjects/Apple.h"
#include "GameObjects/UI.h"

using namespace rdt;

Menu::Menu()
{
}

Menu::~Menu()
{
}

void Menu::OnRegister()
{
	if (m_realms.size() == 0) {
		m_realms.push_back(Physics::CreateRealm());
	}

	Texture& appleTex = TextureManager::LoadTextureFromPNG("apple", "Resources/Textures/apple.png");
	Texture& bananaTex = TextureManager::LoadTextureFromPNG("banana", "Resources/Textures/banana.png");

	Texture& fontTex = TextureManager::LoadTextureFromPNG("font", "Resources/Textures/font.png");
	fontTex.DefineTextureAtlas(28, 28, 15, 3, 4);


	Apple* apple;
	m_game_objects.push_back(apple = new Apple);
	apple->RegisterToRealm(m_realms.back());

	Apple* banana;
	m_game_objects.push_back(banana = new Apple);
	banana->RegisterToRealm(m_realms.back());
	banana->SetTexture("banana");




	UI* highScore;
	m_game_objects.push_back(highScore = new UI(UI_Text, 10, { 18, 18 }));
	highScore->RegisterToRealm(m_realms.back());
	highScore->SetOrigin({ SCREEN_WIDTH / 2 - 75, SCREEN_HEIGHT - 100 });
	highScore->SetAlignment(TEXT_LEFT);
	highScore->SetText("HIGH SCORE");
	highScore->SetLayer(0);
	
}

void Menu::OnBind()
{
	for (auto& gobject : m_game_objects) {
		gobject->OnBind();
	}
	((Apple*)m_game_objects[1])->MoveTo({ 120, 600 });

	Physics::ActivateRealm(m_realms[0]);
}

void Menu::OnProcessInput(const float deltaTime)
{
	RunProcessInputQueue(deltaTime);
}

void Menu::OnRelease()
{
}

void Menu::OnRender()
{
	RunRenderQueue();
}

void Menu::OnMessage(rdt::Message msg)
{
}
