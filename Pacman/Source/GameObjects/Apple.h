#pragma once
#include "Game.h"

class Apple : public rdt::GameObject {
private:
	std::string m_texName;
public:
	Apple();
	~Apple();

	void OnBind() override final;
	void OnRelease() override final;
	void OnProcessInput(const float deltaTIme) override final;
	void OnFinalUpdate() override final;
	void OnRender() override final;
	void OnMessage(rdt::Message msg) override final;

	void SetTexture(const std::string& texName);
	void MoveTo(rdt::Vec2d nPos);

};