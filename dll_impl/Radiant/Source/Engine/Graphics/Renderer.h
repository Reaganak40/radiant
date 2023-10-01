#pragma once
#include "Core.h"

#include "Utils/MathTypes.h"
#include "Utils/UniqueID.h"
#include "Utils/Color.h"

#ifdef CreateWindow
#undef CreateWindow
#endif

namespace rdt {

	class RADIANT_API Renderer {
	protected:
		Renderer();
		virtual ~Renderer();
	private:
		static Renderer* m_instance;
	public:

		/*
			Intiializes a new renderer singleton context.
		*/
		static void Initialize();

		/*
			Destroys the current renderer singleton context.
		*/
		static void Destroy();

		/*
			Creates a new GLFW window with the provided specifications.
		*/
		static Vec2i CreateWindow(const std::string& windowName, unsigned int windowWidth, unsigned int windowHeight, bool resizable) { return m_instance->CreateWindowImpl(windowName, windowWidth, windowHeight, resizable); }

		// *****************************************************
		// 
		//			        Renderer API Calls
		// 
		// *****************************************************

		/*
			Returns true if the window has closed and program should terminate.
		*/
		static bool ShouldWindowClose() { return m_instance->ShouldWindowCloseImpl(); }

		/*
			Gets the current window instance's width
		*/
		static unsigned int GetWindowWidth() { return m_instance->GetWindowWidthImpl(); }

		/*
			Gets the current window instance's height
		*/
		static unsigned int GetWindowHeight() { return m_instance->GetWindowHeightImpl(); }

		/*
			Gets the 2D camera coordinates based in the bottom left corner.
		*/
		static Vec2d GetCameraCoordinates2D() { return m_instance->GetCameraCoordinates2DImpl(); }

		/*
			Sets the background color used on clear.
		*/
		static void SetBackgroundColor(const Color& color) { m_instance->SetBackgroundColorImpl(color); }



		// *****************************************************
		// 
		//			     Renderer Core Engine Calls
		// 
		// *****************************************************
		/*
			Removes a polygon from renderer mesh cache.
		*/
		static void _FlushPolygon(const UniqueID UUID) { m_instance->_FlushPolygonImpl(UUID); }

	protected:
		// Implementation Functions to be implemented by the proper platform.

		virtual bool ShouldWindowCloseImpl() = 0;
		virtual unsigned int GetWindowWidthImpl() = 0;
		virtual unsigned int GetWindowHeightImpl() = 0;
		virtual Vec2i CreateWindowImpl(const std::string& windowName, unsigned int windowWidth, unsigned int windowHeight, bool resizable) = 0;
		virtual Vec2d GetCameraCoordinates2DImpl() = 0;
		virtual void SetBackgroundColorImpl(const Color& color) = 0;
		virtual void _FlushPolygonImpl(const UniqueID UUID) = 0;
	};
}