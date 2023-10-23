#pragma once
#include "Core.h"

// For Render API
#include "Polygon/Polygon.h"
#include "Polygon/Rect.h"
#include "Polygon/Line.h"
#include "Mesh.h"
#include "Utils/Color.h"
#include "Texture/TextureManager.h"
#include "RenderTypes.h"
#include "Gui/Gui.h"
#include "RenderWindow.h"

#include "Camera.h"

namespace rdt {

	class RADIANT_API Renderer {
	private:
		struct Impl;
		Impl* m_impl;

	protected:
		Renderer();
		virtual ~Renderer();

		/*
			Gets the binded render windows
		*/
		std::unordered_map<int, RenderWindow*>& GetRenderWindows();

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
		static Vec2i OnWindowResize() { return m_instance->OnWindowResizeImpl(); }

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
		static void Render() { m_instance->RenderImpl(); }

		/*
			Polls render events and flushes the command queue and buffers.
		*/
		static void OnEndFrame() { m_instance->OnEndFrameImpl(); }

		/*
			Utility function for quickly drawing a Rect to the screen.
		*/
		static void DrawRect(const Vec2d& origin, const Vec2d& size, const Color& color,
			unsigned int layer = 0) {
			m_instance->DrawRectImpl(origin, size, color, layer);
		}

		/*
			Utility function for quickly drawing a Line to the screen.
		*/
		static void DrawLine(const Vec2d& start, const Vec2d& end, const Color& color,
			unsigned int layer = 0) {
			m_instance->DrawLineImpl(start, end, color, layer);
		}

		/*
			Starts a new render command session context.
		*/
		static void Begin(unsigned int layer = 0) { m_instance->BeginImpl(layer); }

		/*
			Ends the current render command session context.
		*/
		static void End() { m_instance->EndImpl(); }

		/*
			Sets the render condition for the current context.
		*/
		static void SetRenderType(core::RenderType type) { m_instance->SetRenderTypeImpl(type); }

		/*
			Adds a polygon to the render queue to be drawn next frame. Use optional
			offset to move the origin of the polygon an arbitary distance from its
			current location.
		*/
		static void AddPolygon(const Polygon& polygon, const Vec2f& offset = Vec2f::Zero()) { m_instance->AddPolygonImpl(polygon, offset); }

		/*
			Adds a rect polygon to the next draw frame. This effectively calls
			AddPolygon but without the need for a stored polygon reference.
		*/
		static void AddRect(const Vec2d& origin, const Vec2d& size, const Vec2f& offset = Vec2f::Zero()) { m_instance->AddRectImpl(origin, size, offset); }

		/*
			Adds a line to the render queue to be drawn next frame.
		*/
		static void AddLine(const Line& line) { m_instance->AddLineImpl(line); }

		/*
			Sets the draw color for lines, which will be used on the next line.
		*/
		static void SetLineColor(const Color& color) { m_instance->SetLineColorImpl(color); }

		/*
			Sets the draw color for lines, which will be used on the next line.
		*/
		static void SetLineColor(ColorType color) { m_instance->SetLineColorImpl(color); }

		/*
			Applies a post (applied later) rotation to the next polygon when it is drawn.
		*/
		static void SetPolygonRotation(const float radians) { m_instance->SetPolygonRotationImpl(radians); }

		/*
			Sets the draw color for polygons, which will be used on the next polygon.
		*/
		static void SetPolygonColor(const Color& color) { m_instance->SetPolygonColorImpl(color); }

		/*
			Sets the draw color for polygons, which will be used on the next polygon.
		*/
		static void SetPolygonColor(ColorType color) { m_instance->SetPolygonColorImpl(color); }

		/*
			Sets the textureID for the next polygon. This ID should correspond with
			an already loaded texture in the TextureManager. If the texture has a
			defined atlas, provide the tile coordinate index.

			This currently only works for Rects.
		*/
		static void SetPolygonTexture(const std::string& texName, unsigned int atlasX = 0, unsigned int atlasY = 0) { m_instance->SetPolygonTextureImpl(texName, atlasX, atlasY); }


		/*
			Flips the texture coordinates for the next polygon, horizontally.
		*/
		static void FlipPolygonTextureHorizontal(bool flip = true) { m_instance->FlipPolygonTextureHorizontalImpl(flip); }

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
		static Vec2d ScreenToWorldCoordinates(const Vec2d& ScreenCoords, int renderWindowIndex = -1) { return m_instance->ScreenToWorldCoordinatesImpl(ScreenCoords, renderWindowIndex); }

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
		static Vec2d _TranslateMouseCoordsToViewport(const Vec2d& mouseCoords, int renderWindowIndex = -1) { return m_instance->_TranslateMouseCoordsToViewportImpl(mouseCoords, renderWindowIndex); }

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

		virtual void ClearImpl() = 0;
		virtual void OnBeginFrameImpl() = 0;
		virtual void OnUpdateImpl(const float deltaTime) = 0;
		virtual void RenderImpl() = 0;
		virtual void OnEndFrameImpl() = 0;
		virtual void DrawRectImpl(const Vec2d& origin, const Vec2d& size, const Color& color, unsigned int layer = 0) = 0;
		virtual void DrawLineImpl(const Vec2d& start, const Vec2d& end, const Color& color, unsigned int layer = 0) = 0;
		virtual void BeginImpl(unsigned int layer) = 0;
		virtual void EndImpl() = 0;

		virtual void SetRenderTypeImpl(core::RenderType type) = 0;
		virtual void AddPolygonImpl(const Polygon& polygon, const Vec2f& offset) = 0;
		virtual void AddRectImpl(const Vec2d& origin, const Vec2d& size, const Vec2f& offset) = 0;
		virtual void AddLineImpl(const Line& line) = 0;
		virtual void SetLineColorImpl(const Color& color) = 0;
		virtual void SetPolygonColorImpl(const Color& color) = 0;
		virtual void SetPolygonRotationImpl(const float radians) = 0;
		virtual void SetPolygonTextureImpl(const std::string& texName, unsigned int atlasX = 0, unsigned int atlasY = 0) = 0;
		virtual void FlipPolygonTextureHorizontalImpl(bool flip) = 0;
		virtual void AttachGuiImpl(GuiTemplate* gui) = 0;
		virtual void DetachGuiImpl(const GuiTemplate* gui) = 0;

		virtual Vec2d ScreenToWorldCoordinatesImpl(const Vec2d& ScreenCoords, int renderWindowIndex) = 0;

		virtual void _FlushPolygonImpl(const UniqueID UUID) = 0;
		virtual Vec2d _TranslateMouseCoordsToViewportImpl(const Vec2d& mouseCoords, int renderWindowIndex) = 0;
	};
}