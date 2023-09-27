#include "Sandbox.h"

class World : public rdt::Scene {
private:
	rdt::SoundID m_sound1, m_sound2;
public:
	World();
	~World();

	void OnRegister() override final;
	void OnProcessInput(float deltaTime) override final;
};