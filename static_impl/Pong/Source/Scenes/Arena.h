#include "Pong.h"

class Arena : public rdt::Scene {
private:
	bool previously_bounded;
	bool computerActive;
public:
	Arena();
	~Arena();

	void OnRegister() override final;
	void OnBind() override final;
	void OnRelease() override final;

	void OnRender() override final;

	void SetComputerActive(bool active);
};