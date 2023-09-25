#include "Sandbox.h"

class World : public rdt::Scene {
private:
	rdt::SoundID m_sound;
public:
	World();
	~World();

	void OnRegister() override final;
	void OnProcessInput(float deltaTime) override final;
};