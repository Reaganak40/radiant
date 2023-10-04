#pragma once
#include "Core.h"
#include "Utils/MathTypes.h"
#include "Utils/Color.h"

namespace rdt {
	enum AspectRatio {
		AR_16_9
	};

	class RADIANT_API Camera {
	private:
		glm::mat4 m_proj;
		glm::mat4 m_view;
		glm::mat4 m_model;

		Vec2i m_viewportPos;
		Vec2i m_viewportSize;

		Vec2d m_aspectRatio;

		Vec2d m_worldCoords;

		Color m_background_color;
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
		void SetViewport(const Vec2i& viewportPos, const Vec2i& viewportSize);

		/*
			Gets the viewport from the camera.
		*/
		void GetViewport(int* viewportPosX, int* viewportPosY, int* viewportWidth, int* viewportHeight);

		/*
			Sets the background draw color that is used on clear.
		*/
		void SetBackgroundColor(const Color& nColor);

		/*
			Returns the currently bounded background color for this camera.
		*/
		const Color& GetBackgroundColor();
	};
}