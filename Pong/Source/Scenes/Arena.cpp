#include "Arena.h"

#include "GameObjects/Padel.h"
#include "GameObjects/Ball.h"
#include "GameObjects/Wall.h"
#include "UI/Scoreboard.h"
#include "UI/ReturnMenu.h"

Arena::Arena()
    : previously_bounded(false)
{
}

Arena::~Arena()
{
}

void Arena::OnRegister()
{
    using namespace Radiant;

    if (m_realms.size() == 0) {
        m_realms.push_back(Physics::CreateRealm());
    }

    UniqueID m_realmID = m_realms[0];

    Padel* player1;
    m_game_objects.push_back(player1 = new Padel(30 + (PADEL_WIDTH / 2), WORLD_HEIGHT / 2));
    player1->RegisterToScene(GetID());
    player1->RegisterToRealm(m_realmID);
    player1->SetRightControl({});
    player1->SetLeftControl({});

    Padel* player2;
    m_game_objects.push_back(player2 = new Padel(WORLD_WIDTH - (PADEL_WIDTH / 2) - 30, WORLD_HEIGHT / 2));
    player2->RegisterToScene(GetID());
    player2->RegisterToRealm(m_realmID);
    player2->SetUpControl(std::vector<InputState>{UP_KEY_DOWN, UP_KEY_PRESS});
    player2->SetDownControl(std::vector<InputState>{DOWN_KEY_DOWN, DOWN_KEY_PRESS});
    player2->SetRightControl({});
    player2->SetLeftControl({});


    Ball* ball;
    m_game_objects.push_back(ball = new Ball(WORLD_WIDTH * 0.85, WORLD_HEIGHT / 2));
    ball->RegisterToRealm(m_realmID);
    ball->RegisterToScene(GetID());


    Wall* top;
    m_game_objects.push_back(top = new Wall(WORLD_WIDTH / 2, WORLD_HEIGHT + (WALL_HEIGHT / 2)));
    top->RegisterToRealm(m_realmID);
    top->RegisterToScene(GetID());
    top->SetWallVisibility(true);

    Wall* bottom;
    m_game_objects.push_back(bottom = new Wall(WORLD_WIDTH / 2, WALL_HEIGHT / 2 + 60));
    bottom->RegisterToRealm(m_realmID);
    bottom->RegisterToScene(GetID());
    bottom->SetWallVisibility(true);

    m_GUIs.push_back(new Scoreboard(*ball));
    m_GUIs.push_back(new ReturnMenu);
}

void Arena::OnBind()
{
    using namespace Radiant;
    Renderer::SetBackgroundColor(BLACK);

    if (!previously_bounded) {
        for (auto& object : m_game_objects) {
            object->OnBind();
        }
        previously_bounded = true;
    }

    for (auto& gui : m_GUIs) {
        Renderer::AttachGui(gui);
    }

    for (auto& realmID : m_realms) {
        Physics::ActivateRealm(realmID);
    }
}

void Arena::OnRelease()
{
    using namespace Radiant;

    for (auto& object : m_game_objects) {
        object->OnRelease();
    }

    for (auto& gui : m_GUIs) {
        Renderer::DetachGui(gui);
    }

    for (auto& realmID : m_realms) {
        Physics::DeactivateRealm(realmID);
    }

    ((Ball*)m_game_objects[2])->Reset();
    ((ReturnMenu*)m_GUIs[1])->Reset();
}

void Arena::OnRender()
{
    RunRenderQueue();

    if (((ReturnMenu*)m_GUIs[1])->RequestedMenu()) {
        ChangeScene("MainMenu");
    }
}
