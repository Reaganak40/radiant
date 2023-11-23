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
	using ViewportID = unsigned int;

	using TextureSlot = unsigned int;
	using TextureID = unsigned int;
	class Texture;

	using CameraID = unsigned int;
	class Camera;

	struct Vertex;
}

namespace rdt::glCore {

	using WindowImpl = void;

	class GL_CORE_API glApplication {
	private:
		char* m_name;
		WindowImpl* m_windowImpl;

		glApplication(const char* name);
		~glApplication();
	public:

		/*
			Gets the registered name of the application. Returns empty
			string if not registered.
		*/
		const char* GetName();

		/*
			Launches a new OpenGL window, using the provided window name, or
			uses the instance name if no windowName provided.
		*/
		bool LaunchWindow();

		/*
			Returns true if the application should terminate (close window requested)
		*/
		bool AppShouldClose();

		/*
			Runs start-up procedures for the beginning of a new draw frame
		*/
		void BeginFrame();

		/*
			Clears the screen (the area of the enabled viewport)
		*/
		void Clear();

		/*
			Sets the RGBA values of the clear color (normalized)
		*/
		void SetClearColor(float r, float g, float b, float a);

		/*
			Runs end of frame procedures for a draw frame (swapping buffers, polling input)
		*/
		void EndFrame();

		/*
			Creates a new viewport that can binded and drawn too. Use the
			returned viewport's ID to reference it later.
		*/
		ViewportID CreateViewport(int xPos, int yPos, int width, int height);

		/*
			Binds the viewport for the current draw call, if no ID
			provided (0), uses the default viewport.
		*/
		void SetViewport(ViewportID vID = 0);

		/*
			Loads a texture from memory (in PNG format) to be applied to OpenGL
			objects. Returns its TextureID to be referenced later.
		*/
		TextureID CreateTextureFromPNG(const char* filepath);

		/*
			Returns a constant reference to a registered texture. If no texture
			matches the textureID given, will return the None texture.
		*/
		const Texture& GetTexture(TextureID tID);

		/*
			Notifies the draw context that the texture (indicated by the TextureID) will
			be used in the draw call.

			Vertex data that used this TextureID for its texIndex will be interrogated and
			modified to map to the texture's map index.
		*/
		void UseTexture(TextureID tID);

		/*
			Sets the model view projection matrix to be used by the selected
			shader.
		*/
		void SetMVPMatrix(const glm::mat4& mvp);

		/*
			Sets the aspect ratio that is to be maintained by the viewport.
			If the viewport does not match the aspect ratio, the viewport
			will shrink to maximum fit.
		*/
		void SetAspectRatio(AspectRatio aspectRatio);

		/*
			Submits an array of vertices for the current draw call. This
			can be called multiple times per draw context.
		*/
		void SubmitVertices(Vertex* vertices, size_t vertexCount);

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

		friend class glState;

	private:
	};

	/*
		Creates a new instance of a OpenGL application. Registers the instance
		with the internal glState, for global reference.
	*/
	GL_CORE_API glApplication* CreateApplication(const char* application_name);

	/*
		Returns a pointer to a OpenGL created instance. If none
		exists under the provided name, it will be created.
	*/
	GL_CORE_API glApplication* GetApplication(const char* application_name);

	/*
		Destroys an instantiated OpenGL application. If it has a name,
		will be removed from the instance map. Returns true if successful.
	*/
	GL_CORE_API bool DestroyApplication(glApplication& application);
}