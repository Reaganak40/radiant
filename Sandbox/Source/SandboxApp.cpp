#include <Radiant.h>
#include <Engine/EntryPoint.h>

#include "Scenes/Sandbox.h"

using namespace rdt;

class PinkBox : public EntityDefinition {
private:
	Vec2d spawnPos;
public:
	PinkBox(double x, double y)
	{
		spawnPos.x = x;
		spawnPos.y = y;
	}

	void OnCreate() override final
	{
		Register();
		Entity eID = GetID();

		RigidBody2D rigidBody;
		rigidBody.polygon = std::make_shared<Rect>(spawnPos, 50, 50);
		EntityManager::AddComponent<RigidBody2D>(eID, rigidBody);

		Renderable renderable;
		renderable.polygon_color = PINK;
		EntityManager::AddComponent<Renderable>(eID, renderable);
		Renderable::AddPolygonFromRigidBody(eID);

		SystemManager::AddEntity<PhysicsSystem>(eID);
		SystemManager::AddEntity<RenderSystem>(eID);
	}
};

class SandboxApp : public Application
{
public:
	SandboxApp()
	{
		// Snake Application Configurations
		ApplicationConfig appConfig;
		appConfig.appName = "Sandbox";
		appConfig.windowWidth = 1280;
		appConfig.windowHeight = 720;
		appConfig.backgroundColor = WHITE;
		appConfig.cameraAspectRatio = AR_16_9;
		SetApplicationConfig(appConfig);
	}

	void OnGameBegin() override final
	{
		for (int i = 1; i < 10; i++) {
			EntityFactory::Create(new PinkBox(100 * i, 100));
		}

		AddScene("Sandbox Scene", new Sandbox);
		SetScene("Sandbox Scene");
	}

	~SandboxApp()
	{

	}

};

rdt::Application* rdt::CreateApplication()
{
	return new SandboxApp;
}