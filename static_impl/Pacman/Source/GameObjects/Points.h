#pragma once
#include "Game.h"

struct PointData {
	unsigned int pointVal;
	rdt::Vec2d pos;

	PointData(unsigned int nPointVal, rdt::Vec2d nPos = {FRUIT_POS_X, FRUIT_POS_Y})
		: pointVal(nPointVal), pos(nPos) {}
};

class Points : public rdt::GameObject {
private:
	rdt::Vec2d spawnPos;
	unsigned int m_pointVal;
	unsigned int m_atlasX;
	unsigned int m_atlasY;
	rdt::Color m_color;

	bool shouldShow;

	rdt::Timer m_show_timer;
public:
	Points();
	~Points();

	void OnBind() override final;
	void OnRelease() override final;
	void OnProcessInput(const float deltaTime) override final;
	void OnFinalUpdate() override final;
	void OnRender() override final;
	void OnMessage(rdt::Message msg) override final;
private:
	void OnShow(PointData* data);
	void SetPointVal(unsigned int nVal);
	void SetAtlasCoordinates();
};