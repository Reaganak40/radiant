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

	using ViewportID = unsigned int;

	class Viewport {
	private:
		ViewportID mID;
	public:

		int posX = 0;
		int posY = 0;
		int width = 0;
		int height = 0;
		
		Viewport(int nPosX = 0, int nPosY = 0, int nWidth = 0, int nHeight = 0);
		~Viewport();

		/*
			Returns the registered viewportID for this viewport
		*/
		ViewportID GetID() const;

		/*
			Sets the pixel position of the viewport, origin in the top-left corner
		*/
		void SetPosition(int x, int y);

		/*
			Srts the pixel dimensions of the viewport.
		*/
		void SetSize(int width, int height);
		
		/*
			Binds the viewport to the OpenGL context
		*/
		void Bind() const;

		/*
			Binds a viewport to the OpenGL context without the need
			of a viewport instance.
		*/
		static void Bind(int xPos, int yPos, int width, int height);

		friend class Window;
	};
}