#include "Pong.h"

class Arena : public Radiant::Scene {
private:
	bool previously_bounded;
public:
	Arena();
	~Arena();

	void OnRegister() override final;
	void OnBind() override final;
	void OnRelease() override final;

	void OnRender() override final;

};