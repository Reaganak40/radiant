#pragma once
#include "Game.h"
#include "Pacman.h"

enum FRUIT_TYPE {
	CHERRY,
	STRAWBERRY,
	PEACH,
	APPLE,
	MELON,
	GALAXIAN,
	BELL,
	KEY
};

struct FruitData {
	FRUIT_TYPE type;
	bool isSecondCapture;

	FruitData(FRUIT_TYPE nType, bool nIsSecondCapture = false)
		: type(nType), isSecondCapture(nIsSecondCapture) {}

	unsigned int GetValue() {
		switch (type) {
		case CHERRY:
			return isSecondCapture ? 200 : 100;
		case STRAWBERRY:
			return isSecondCapture ? 400 : 300;
		case PEACH:
			return isSecondCapture ? 600 : 500;
		case APPLE:
			return isSecondCapture ? 800 : 700;
		case MELON:
			return isSecondCapture ? 1500 : 1000;
		case GALAXIAN:
			return isSecondCapture ? 2500 : 2000;
		case BELL:
			return isSecondCapture ? 4000 : 3000;
		case KEY:
			return 5000;
		default:
			break;
		}

		return 0;
	}
};

class Fruit : public rdt::GameObject {
private:
	FRUIT_TYPE m_type;
	bool m_shouldShow;
	bool m_eaten_once;

	Pacman* m_pacman_ptr;

	rdt::Timer m_show_timer;
public:
	Fruit();
	~Fruit();

	void OnBind() override final;
	void OnRelease() override final;
	void OnProcessInput(const float deltaTIme) override final;
	void OnFinalUpdate() override final;
	void OnRender() override final;
	void OnMessage(rdt::Message msg) override final;

	void SetType(FRUIT_TYPE type);
private:
	void AddGameObjectPtr(rdt::MessageID from, rdt::GameObjectPtrData* data);
	void ResolveCollision(rdt::CollisionData* data);
	void OnShowFruit();
	void OnRespawn();
};