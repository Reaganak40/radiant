#include "pch.h"
#include "Viewport.h"

glCore::Viewport::Viewport(int nPosX, int nPosY, int nWidth, int nHeight)
	: posX(nPosX), posY(nPosY), width(nWidth), height(nHeight)
{
	mID = GL_CORE_DEFAULT_VIEWPORT_ID;
}

glCore::Viewport::~Viewport()
{
}

glCore::ViewportID glCore::Viewport::GetID() const
{
	return mID;
}

void glCore::Viewport::SetPosition(int x, int y)
{
	posX = x;
	posY = y;
}

void glCore::Viewport::SetSize(int nWidth, int nHeight)
{
	width = nWidth;
	height = nHeight;
}

void glCore::Viewport::Bind() const
{
	glViewport(posX, posY, width, height);
}

void glCore::Viewport::Bind(int xPos, int yPos, int width, int height)
{
	glViewport(xPos, yPos, width, height);
}
