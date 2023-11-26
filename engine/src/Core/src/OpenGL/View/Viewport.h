/*******************************************************************
*	File:    Viewport.h
*	Module:  Internal
*
*	Author: Reagan Kelley
*
*	Viewports defines the where on the window to render from, 
*	from its origin to its dimensions.
*******************************************************************/
#pragma once

#define GL_CORE_DEFAULT_VIEWPORT_ID 0
namespace rdt::glCore {

	struct Viewport
	{
		int posX = 0;
		int posY = 0;
		int width = 0;
		int height = 0;
		
		Viewport(int nPosX = 0, int nPosY = 0, int nWidth = 0, int nHeight = 0);
		
		/*
			Binds the viewport to the OpenGL context
		*/
		void Bind();

	private:
		static Viewport m_binded_viewport;
	};
}