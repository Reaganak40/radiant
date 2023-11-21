/*******************************************************************
*	Module: Camera
*
*	This section includes the class declaration of Camera, an
*	interface that describes the view into the game world. In
*	also defines MVP matrices used by the shader.
*******************************************************************/
#pragma once
#include "gl_core_api.h"

#define GL_CORE_NULL_CAMERA_ID 0

namespace rdt::glCore {
	enum AspectRatio {
		AR_16_9, // Window Projection = (1920px, 1080px)
		AR_1_1,  // Window Projection = (1000px, 1000px)
	};

	using CameraID = unsigned int;
	using GLM_IMPL_MAT4 = void;

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