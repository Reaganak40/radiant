#pragma once
#include "Core.h"
#include "Utils/MathTypes.h"
#include "Utils/Color.h"

namespace rdt {
	enum AspectRatio {
		AR_16_9 // Window Projection = (1920px, 1080px)
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

		glm::mat4 GetMVP();

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