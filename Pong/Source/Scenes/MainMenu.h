#include "Pong.h"

class MainMenu : public Radiant::Scene {
private:
public:
	MainMenu();
	~MainMenu();

	void OnRegister() override final;
	void OnBind() override final;
	void OnRelease() override final;
};