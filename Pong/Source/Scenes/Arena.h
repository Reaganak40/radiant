#include "Pong.h"

class Arena : public Radiant::Scene {
private:
public:
	Arena();
	~Arena();

	void OnRegister() override final;
	void OnBind() override final;
	void OnRelease() override final;

};