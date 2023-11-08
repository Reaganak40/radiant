/*******************************************************************
*	Module:  Graphics (API)
*	File:    Renderer.h
*
*	Author: Reagan Kelley
*
*	The Render singleton is an API that allows the client and engine
*	communicate with the interior rendering framework. Can be
*	used to create render command queues, to setup and draw
*	frames.
*******************************************************************/
#pragma once
#include "Core.h"

// Forward Declarations
namespace rdt {
	namespace core 
	{
		struct Mesh;
		enum RenderType;
	}
	struct Mesh;
	using MeshBuffer = std::vector<core::Mesh>;
	using ModelID = unsigned int;
	using UniqueID = unsigned int;
	using TextureID = unsigned int;   // Unique Identifier for a Texture
	enum ColorType;
	struct Transform;
	struct AtlasProfile;
	struct Vec2d;
	struct Vec2i;
	class GuiTemplate;
	class RenderWindow;
	class Color;
	class Camera;
	class Line;
	class Polygon;
}

namespace rdt {

	class RADIANT_API Renderer {
	private:
		struct Impl;
		static Impl* m_impl;

	protected:
		Renderer();
		virtual ~Renderer();

		/*
			Gets the binded render windows
		*/
		std::unordered_map<int, RenderWindow*>& GetRenderWindows();

		/*
			Sets the fullscreen flag to true (enabled), or false (disabled)
		*/
		void SetFullscreenFlag(bool isFullscreen);


		/*
			Gets the buffer that should be used in the current render call.
		*/
		MeshBuffer& GetBackBuffer();

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
		static bool CreateRadiantWindow(const std::string& windowName) { return m_instance->CreateWindowImpl(windowName); }

		/*
			Gets the platform window instance (currently just GLFW)
		*/
		static void* GetWindowInstance() { return m_instance->GetWindowInstanceImpl(); }

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
			Sets the background color used on clear.
		*/
		static void SetBackgroundColor(const Color& color) { m_instance->SetBackgroundColorImpl(color); }

		/*
			Update renderer internals if encountered a window resize event. Returns
			the aspect ratio.
		*/
		static Vec2i OnWindowResize();

		/*
			Adds a new render window to the rendering context. The renderer will draw
			frames in the render window. Returns the location of it, to be removed
			later.
		*/
		static int AddRenderWindow(RenderWindow* nRenderWindow);

		/*
			Clears the screen with the defined background color.
		*/
		static void Clear() { m_instance->ClearImpl(); }

		/*
			Runs start of frame procedures for the renderer.
		*/
		static void OnBeginFrame() { m_instance->OnBeginFrameImpl(); }

		/*
			Runs the renderer OnUpdate step.
		*/
		static void OnUpdate(const float deltaTime) { m_instance->OnUpdateImpl(deltaTime); }

		/*
			Runs the draw command queue, drawing all objects to the screen.
		*/
		static void Render();

		/*
			Polls render events and flushes the command queue and buffers.
		*/
		static void OnEndFrame() { m_instance->OnEndFrameImpl(); }

		/*
			Starts a new render command session context.
		*/
		static void Begin(unsigned int layer = 0);

		/*
			Ends the current render command session context.
		*/
		static void End();
		
		/*
			Sets the model to use for this draw call
		*/
		static void SetModel(ModelID modelID);

		/*
			Sets the transform to be applied to the target model.
		*/
		static void SetTransform(const Transform& transform);

		/*
			Sets the texture to be applied to the mesh's model.
		*/
		static void SetTexture(TextureID texture);

		/*
			Sets the atlas profile to be applied to the texture.
		*/
		static void SetAtlasProfile(const AtlasProfile& profile);

		/*
			Horizontally flips the texture
		*/
		static void FlipTextureHorizontal();

		/*
			Sets the filled color of the mesh's model.
		*/
		static void SetFillColor(const Color& color);

		/*
			Sets the render condition for the current context.
		*/
		static void SetRenderType(core::RenderType type) { m_instance->SetRenderTypeImpl(type); }

		/*
			Attaches a Gui instance, which will be drawn at the end of the draw command queue.
			The Renderer is not responsible for freeing the GUI instance. It should be detached
			and freed by its owner.
		*/
		static void AttachGui(GuiTemplate* gui) { m_instance->AttachGuiImpl(gui); }

		/*
			Removes a GUI that is currently being rendered each frame.
		*/
		static void DetachGui(const GuiTemplate* gui) { m_instance->DetachGuiImpl(gui); }

		/*
			Gets a camera pointer from the camera map.
		*/
		static Camera& GetCamera();

		/*
			Sets the default viewport to be used or not. When it is used, the game window
			will fill to the dimensions of the application window. If false, the game window
			will be rendered within registered RenderWindows.
		*/
		static void SetDefaultViewport(bool use);

		/*
			Makes the window fullscreen
		*/
		static void EnableFullscreen() { m_instance->EnableFullscreenImpl(); }

		/*
			Restores the window back to windowed mdoe
		*/
		static void DisableFullscreen() { m_instance->DisableFullscreenImpl(); }

		/*
			Returns true if the window is currently in fullscreen.
		*/
		static bool IsFullscreen();

		/*
			Returns true if the default viewport should be used for render windows.
		*/
		static bool UsingDefaultViewport();

		/*
			Returns true if the provided polygon will be seen by the currently bounded
			camera.
		*/
		static bool IsInView(const Polygon& polygon);

		/*
			Returns true if the provided rect will be seen by the currently bounded
			camera.
		*/
		static bool IsInView(const Vec2d& rectOrigin, const Vec2d& rectSize);

		/*
		Returns the world coordinates from provided screen coordinates on the camera.
		*/
		static Vec2d ScreenToWorldCoordinates(const Vec2d& ScreenCoords, int renderWindowIndex = -1);

		// *****************************************************
		// 
		//			     Renderer Core Engine Calls
		// 
		// *****************************************************
		/*
			Removes a polygon from renderer mesh cache.
		*/
		static void _FlushPolygon(const UniqueID UUID);

		/*
			Takes in mouse coordinates and returns them scopes relative to the game window viewport.
			If not using default viewport, specify the render window by its id/index to get the 
			coordinate offset from that window.
		*/
		static Vec2d _TranslateMouseCoordsToViewport(const Vec2d& mouseCoords, int renderWindowIndex = -1);

	protected:
		// Implementation Functions to be implemented by the proper platform.

		virtual bool ShouldWindowCloseImpl() = 0;
		virtual unsigned int GetWindowWidthImpl() = 0;
		virtual unsigned int GetWindowHeightImpl() = 0;
		virtual bool CreateWindowImpl(const std::string& windowName) = 0;
		virtual void* GetWindowInstanceImpl() = 0;
		virtual void SetBackgroundColorImpl(const Color& color) = 0;
		virtual Vec2i OnWindowResizeImpl() = 0;
		virtual void OnNewRenderWindow(int id, RenderWindow* nRenderWindow) = 0;
		virtual void EnableFullscreenImpl() = 0;
		virtual void DisableFullscreenImpl() = 0;

		virtual void ClearImpl() = 0;
		virtual void OnBeginFrameImpl() = 0;
		virtual void OnUpdateImpl(const float deltaTime) = 0;
		virtual void RenderImpl() = 0;
		virtual void OnEndFrameImpl() = 0;

		virtual void SetRenderTypeImpl(core::RenderType type) = 0;

		virtual void AttachGuiImpl(GuiTemplate* gui) = 0;
		virtual void DetachGuiImpl(const GuiTemplate* gui) = 0;

		virtual Vec2d ScreenToWorldCoordinatesImpl(const Vec2d& ScreenCoords, int renderWindowIndex) = 0;

		virtual void _FlushPolygonImpl(const UniqueID UUID) = 0;
		virtual Vec2d _TranslateMouseCoordsToViewportImpl(const Vec2d& mouseCoords, int renderWindowIndex) = 0;
	};
}