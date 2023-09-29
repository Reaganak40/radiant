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
			return 100;
		case STRAWBERRY:
			return 300;
		case PEACH:
			return 500;
		case APPLE:
			return 700;
		case MELON:
			return 1000;
		case GALAXIAN:
			return 2000;
		case BELL:
			return 3000;
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
	rdt::SoundID m_fruitEatenSound;
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