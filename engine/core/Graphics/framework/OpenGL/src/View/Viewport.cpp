#include "pch.h"
#include "Viewport.h"

rdt::glCore::Viewport rdt::glCore::Viewport::m_binded_viewport;

rdt::glCore::Viewport::Viewport(int nPosX, int nPosY, int nWidth, int nHeight)
	: posX(nPosX), posY(nPosY), width(nWidth), height(nHeight)
{
}

void rdt::glCore::Viewport::Bind()
{
	if (
		m_binded_viewport.posX   != posX  ||
		m_binded_viewport.posY   != posY  ||
		m_binded_viewport.width  != width ||
		m_binded_viewport.height != height
		) {
		
		glViewport(posX, posY, width, height);
		m_binded_viewport.posX   = posX;
		m_binded_viewport.posY   = posY;
		m_binded_viewport.width  = width;
		m_binded_viewport.height = height;
	}
}

