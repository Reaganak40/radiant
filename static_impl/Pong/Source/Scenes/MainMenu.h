#include "Pong.h"

class MainMenu : public rdt::Scene {
private:
public:
	MainMenu();
	~MainMenu();

	void OnRegister() override final;
	void OnBind() override final;
	void OnRelease() override final;

	void OnRender() override final;

};