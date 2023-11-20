/*******************************************************************
*	File:    glCore.h
*	Module:  API
*
*	Author: Reagan Kelley
*
*	This file should be included by client projects to use the
*	glCore library.
*******************************************************************/
#pragma once

#ifdef GL_CORE_PLATFORM_WINDOWS
	#ifdef GL_CORE_BUILD_DLL
		#define GL_CORE_API __declspec(dllexport)
		#define API_EXTERN 
	#else
		#define GL_CORE_API __declspec(dllimport)
		#define API_EXTERN extern
	#endif // GL_CORE_BUILD_DLL
#else
	#error Unsupported Platformn - Windows Only
#endif // !GL_CORE_PLATFORM_WINDOWS

// Forward Declarations
namespace glCore {
	using ViewportID = unsigned int;

	using TextureSlot = unsigned int;
	using TextureID = unsigned int;
	class Texture;

	using CameraID = unsigned int;
	using GLM_IMPL_MAT4 = void;
	class Camera;

	struct Vertex;
}

#define GL_CORE_NULL_TEXTURE_SLOT 0
#define GL_CORE_NONE_TEXTURE 1
#define GL_CORE_NULL_CAMERA_ID 0


namespace glCore {
/*******************************************************************
*	Module:  Application
*
*	Application is the centeral interface to communicate with the
*	glCore internals.
*******************************************************************/
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
			Creates a new camera instance that can be set for draw calls. Cameras
			allow for easy manipulation of the Model View Projection Matrix.
		*/
		CameraID CreateCamera();

		/*
			Gets the camera assoicatied with this CameraID. If it doesn't exist,
			returns the default camera.
		*/
		Camera& GetCamera(CameraID cID);

		/*
			Enables the use of the camera indiciated by the CameraID. If no cameraID provided (0),
			uses the default camera. Only one camera allowed per draw call. This camera will be
			remain the current camera until a different one replaces it.
		*/
		void SetCamera(CameraID cID = 0);

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
	GL_CORE_API glApplication& CreateApplication(const char* application_name);

	/*
		Returns a pointer to a OpenGL created instance. If none
		exists under the provided name, it will be created.
	*/
	GL_CORE_API glApplication& GetApplication(const char* application_name);

	/*
		Destroys an instantiated OpenGL application. If it has a name,
		will be removed from the instance map. Returns true if successful.
	*/
	GL_CORE_API bool DestroyApplication(glApplication& application);

/*******************************************************************
*	Module:  Vertex
*
*	This section contains the Vertex struct, which contains all the
*	data for a single OpenGL vertex point. Used by the client
*	to define geometry, and used throughout the internal rendering
*	pipeline.
*******************************************************************/

	struct GL_CORE_API Vertex {
		float position[3];		// x,y,z coordinats of a vertex
		float color[4];			// the rgba (normalized) color values of a vertex
		float texCoords[2];		// the texture mapping coordinates for this vertex
		float texIndex;         // the texture slot that the target texture is currently binded.

		Vertex();

		void SetPosition(const float nPosition[3]);
		void SetPosition(const float x, const float y, const float z);

		void SetColor(const float nColor[4]);
		void SetColor(const float r, const float g, const float b, const float a);

		void SetTexCoords(const float nTexCoords[2]);
		void SetTexCoords(const float x, const float y);

		void SetTexIndex(float nIndex);
	};

/*******************************************************************
*	Module: Texture
*
*	This section contains the Texture class which holds image 
*	resource data, and openGL bindings.
*******************************************************************/

	class GL_CORE_API Texture {
	private:
		TextureSlot m_texture_slot;
		TextureID m_ID;
		int m_image_width;
		int m_image_height;
		int m_bits_per_pixel;
		int* refCount;
	public:
		Texture();
		Texture(const Texture& other);
		~Texture();

		/*
			Returns the OpenGL texture ID for this instance.
		*/
		const TextureID GetID() { return m_ID; }

		/*
			Returns the assigned texture slot for this texture.
		*/
		const TextureSlot CurrentTextureSlot() { return m_texture_slot; }

		/*
			Returns the image texture width (in pixels)
		*/
		int GetImageWidth();

		/*
			Returns the image texture height (in pixels)
		*/
		int GetImageHeight();

		friend class TextureManager;
	private:
		/*
			Load texture from a PNG image. Returns true if successful.
		*/
		bool LoadTexturePNG(const char* textureFilePath);

		/*
			Sets this texture to a white square (effectively a blank texture)
		*/
		void SetToNone();

		/*
			Binds this texture to this provided texture slot
		*/
		void Bind(TextureSlot slot);
	};

/*******************************************************************
*	Module: Camera
*
*	This section includes the class declaration of Camera, an
*	interface that describes the view into the game world. In
*	also defines MVP matrices used by the shader.
*******************************************************************/
	enum AspectRatio {
		AR_16_9, // Window Projection = (1920px, 1080px)
		AR_1_1,  // Window Projection = (1000px, 1000px)
	};

	class GL_CORE_API Camera {
	private:
		struct Impl;
		Impl* m_impl;

		CameraID m_ID;
		AspectRatio m_aspectRatio;
		bool maintain_aspect_ratio;
		float windowWidth, windowHeight;
		float pos[3]; //x, y, z

	public:

		Camera(AspectRatio aspectRatio = AR_16_9);
		~Camera();

		/*
			Gets the registered cameraID for this Camera.
		*/
		CameraID GetID();

		/*
			Sets the aspect ratio of the camera
		*/
		void SetAspectRatio(AspectRatio nAspectRatio);

		/*
			Sets the flag to determine if the camera should enforce the aspect
			ratio while in its host viewport. If not, the camera view may
			appear stretched if the viewport does not match the aspect ratio
			of the camera.
		*/
		void SetMaintainAspectRatio(bool maintain = true);

		/*
			Returns true if should maintain aspect ratio
		*/
		bool ShouldMaintainAspectRatio();

		/*
			Centers the camera to focus on the provided 2D coordinates
		*/
		void FocusOn(float x, float y);

		/*
			Returns the world coordinates of the camera from the bottom left corner.
		*/
		void GetPosition(float* outX, float* outY, float* outZ);

		/*
			Returns the world cordinates of the camera from the center of the camera view
		*/
		void GetCenter(float* outX, float* outY, float* outZ);

		/*
			Gets the pixel width of the camera's view
		*/
		float GetCameraWindowWidth();

		/*
			Gets the pixel height of the camera's view
		*/
		float GetCameraWindowHeight();


		/*
			Returns the pixel dimensions for this camera if it was encapsulated by a viewport or
			lesser window and we want to maximize the camera view size while maintaining its aspect ratio.
		*/
		void GetCameraDimensionsFromViewport(int viewportWidth, int viewportHeight, int* outWidth, int* outHeight);

		friend class Window;
	private:

		/*
			Returns the translation coordinates based on the camera coordinates.
		*/
		void GetTranslation(float* outX, float* outY, float* outZ);

		/*
			Returns a void pointer to the glm MVP for this camera
		*/
		GLM_IMPL_MAT4* GetMVP();
	};

}

