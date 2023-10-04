#pragma once
#include "Utils/MathTypes.h"

namespace rdt {
	enum AspectRatio {
		AR_16_9
	};
}

namespace rdt::core {

	class Camera {
	private:
		glm::mat4 m_proj;
		glm::mat4 m_view;
		glm::mat4 m_model;

		Vec2d m_viewportPos;
		Vec2d m_viewportSize;

		Vec2d m_aspectRatio;

		Vec2d m_worldCoords;

	public:

		Camera(AspectRatio aspectRatio = AR_16_9);
		~Camera();

		const glm::mat4& GetProjectionMatrix();
		const glm::mat4& GetViewMatrix();
		const glm::mat4& GetModelMatrix();

		glm::mat4 GetMVP();

		/*
			Sets the viewport dimensions for this camera, which defines
			the window view of the application.
		*/
		void SetViewport(const Vec2d& viewportPos, const Vec2d& viewportSize);

		/*
			Gets the viewport from the camera.
		*/
		void GetViewport(int* viewportPosX, int* viewportPosY, int* viewportWidth, int* viewportHeight);
	};
}