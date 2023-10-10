#pragma once
#include "Core.h"
#include "Utils/MathTypes.h"
#include "Utils/Color.h"

namespace rdt {
	enum AspectRatio {
		AR_16_9, // Window Projection = (1920px, 1080px)
		AR_1_1,  // Window Projection = (1000px, 1000px)
	};

	class RADIANT_API Camera {
	private:
		struct Impl;
		Impl* m_impl;

	public:

		Camera(AspectRatio aspectRatio = AR_16_9);
		~Camera();

		const glm::mat4& GetProjectionMatrix();
		const glm::mat4& GetViewMatrix();
		const glm::mat4& GetModelMatrix();

		/*
			Returns the cameras model view projection matrix
		*/
		glm::mat4 GetMVP();

		/*
			Sets the background draw color that is used on clear.
		*/
		void SetBackgroundColor(const Color& nColor);

		/*
			Returns the currently bounded background color for this camera.
		*/
		const Color& GetBackgroundColor();

		/*
			Sets the aspect ratio of the camera
		*/
		void SetAspectRatio(AspectRatio nAspectRatio);

		/*
			Returns the pixel dimensions for this camera if it was encapsulated by a viewport or
			lesser window and we want to maximize the camera view size while maintaining its aspect ratio.
		*/
		Vec2f GetCameraDimensionsFromViewport(float viewportWidth, float viewportHeight);
	};
}