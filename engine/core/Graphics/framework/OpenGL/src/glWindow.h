/*******************************************************************
*	Module:  Application
*
*	Application is the centeral interface to communicate with the
*	rdt::glCore internals.
*******************************************************************/
#pragma once
#include "gl_core_api.h"

// Forward Declarations
namespace rdt::glCore {
	using glTextureID = unsigned int;
	struct glVertex;
}
namespace rdt {
	class WindowConfig;
	class Color;
	struct Vec2i;
	enum AspectRatio;
}

namespace rdt::glCore {

	class GL_CORE_API glWindow {
	private:
		struct Impl;
		Impl* m_impl;

	public:
		glWindow();
		~glWindow();

		/*
			Gets the registered name of the application.
		*/
		const char* GetName();

		/*
			Launches a new OpenGL window, using the provided window name, or
			uses the instance name if no windowName provided.
		*/
		bool LaunchWindow(std::shared_ptr<WindowConfig> windowConfig);

		/*
			Returns true if the application should terminate (close window requested)
		*/
		bool AppShouldClose();

		/*
			Clears the screen (the area of the enabled viewport)
		*/
		void Clear();

		/*
			Sets the RGBA values of the clear color (normalized)
		*/
		void SetClearColor(const Color& color);

		/*
			Runs start-up procedures for the beginning of a new draw frame
		*/
		void BeginFrame();

		/*
			Runs end of frame procedures for a draw frame (swapping buffers, polling input)
		*/
		void EndFrame();

		/*
			Sets the viewport for the current draw context.
		*/
		void SetViewport(int xPos, int yPos, int width, int height);

		/*
			Loads a texture from memory (in PNG format) to be applied to OpenGL
			objects. Returns its TextureID to be referenced later.

			The passed in outSize reference is changed to the pixel dimensions
			of the texture.
		*/
		glTextureID CreateTextureFromPNG(const char* filepath, Vec2i& outSize);

		/*
			Notifies the draw context that the texture (indicated by the TextureID) will
			be used in the following draw call.
		*/
		void UseTexture(glTextureID tID);

		/*
			Sets the model view projection matrix to be used by the selected
			shader.
		*/
		void SetMVPMatrix(const glm::mat4& mvp);

		/*
			Sets the aspect ratio that is to be maintained by the viewport.
			If no aspect ratio is set, the image will be stretched
			according to the viewport dimensions.
		*/
		void SetAspectRatio(AspectRatio aspectRatio);

		/*
			Submits an array of vertices for the current draw call. This
			can be called multiple times per draw context.
		*/
		void SubmitVertices(glVertex* vertices, size_t vertexCount);

		/*
			Submits an array of indcies for the current draw call. This
			can be called multiple times per draw context.

			The indices should be relative to the last submitted vertices
		*/
		void SubmitIndices(unsigned int* indices, size_t indexCount, size_t vertexCount);

		/*
			Uses the previously submission calls (draw context) to send
			an OpenGL draw call. Resets the context.
		*/
		void Draw();
	};
}