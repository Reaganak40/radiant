#include "pch.h"
#include "Viewport.h"

rdt::glCore::Viewport::Viewport(int nPosX, int nPosY, int nWidth, int nHeight)
	: posX(nPosX), posY(nPosY), width(nWidth), height(nHeight)
{
	mID = GL_CORE_DEFAULT_VIEWPORT_ID;
}

rdt::glCore::Viewport::~Viewport()
{
}

rdt::glCore::ViewportID rdt::glCore::Viewport::GetID() const
{
	return mID;
}

void rdt::glCore::Viewport::SetPosition(int x, int y)
{
	posX = x;
	posY = y;
}

void rdt::glCore::Viewport::SetSize(int nWidth, int nHeight)
{
	width = nWidth;
	height = nHeight;
}

void rdt::glCore::Viewport::Bind() const
{
	glViewport(posX, posY, width, height);
}

void rdt::glCore::Viewport::Bind(int xPos, int yPos, int width, int height)
{
	glViewport(xPos, yPos, width, height);
}
