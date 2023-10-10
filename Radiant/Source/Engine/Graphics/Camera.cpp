#include "pch.h"
#include "Camera.h"
#include "Utils/Utils.h"

namespace rdt {
	
	struct Camera::Impl {
		glm::mat4 m_proj;
		glm::mat4 m_view;
		glm::mat4 m_model;

		AspectRatio m_aspectRatio;
		Vec2d m_worldCoords;

		Color m_background_color;

		Impl(AspectRatio aspectRatio)
		{
			float cameraWindowWidth = 0.0f;
			float cameraWindowHeight = 0.0f;

			m_aspectRatio = aspectRatio;
			switch (aspectRatio) {
			case AR_16_9:
				cameraWindowWidth = 1920.0f;
				cameraWindowHeight = 1080.0f;
				break;
			case AR_1_1:
				cameraWindowWidth = 1000.0f;
				cameraWindowHeight = 1000.0f;
				break;
			}
			m_worldCoords = { 0, 0 };

			m_view = glm::translate(glm::mat4(1.0f), glm::vec3(-m_worldCoords.x, -m_worldCoords.y, 0));
			m_model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
			m_proj = glm::ortho(0.0f, cameraWindowWidth, 0.0f, cameraWindowHeight, -1.0f, 1.0f);
		}

		~Impl()
		{
		}

		glm::mat4 GetMVP()
		{
			return m_proj * m_view * m_model;
		}

		void SetAspectRatio(AspectRatio nAspectRatio)
		{
			float cameraWindowWidth = 0.0f;
			float cameraWindowHeight = 0.0f;

			m_aspectRatio = nAspectRatio;
			switch (nAspectRatio) {
			case AR_16_9:
				cameraWindowWidth = 1920.0f;
				cameraWindowHeight = 1080.0f;
				break;
			case AR_1_1:
				cameraWindowWidth = 1000.0f;
				cameraWindowHeight = 1000.0f;
				break;
			}

			m_proj = glm::ortho(0.0f, cameraWindowWidth, 0.0f, cameraWindowHeight, -1.0f, 1.0f);
		}

		Vec2f GetCameraDimensionsFromViewport(float viewportWidth, float viewportHeight)
		{
			Vec2f dimensions = Vec2f::Zero();

			dimensions.y = viewportHeight;
			switch (m_aspectRatio) {
			case AR_16_9:
				dimensions.x = (16.0f / 9.0f) * dimensions.y;
				break;
			case AR_1_1:
				dimensions.x = dimensions.y;
				break;
			}
			dimensions.x = Utils::ApplyEpsilon(dimensions.x);
			dimensions.y = Utils::ApplyEpsilon(dimensions.y);

			if (dimensions.x > viewportWidth) {
				dimensions.x = viewportWidth;
				switch (m_aspectRatio) {
				case AR_16_9:
					dimensions.y = (9 / 16.0f) * dimensions.x;
					break;
				case AR_1_1:
					dimensions.y = dimensions.x;
					break;
				}
				dimensions.x = Utils::ApplyEpsilon(dimensions.x);
				dimensions.y = Utils::ApplyEpsilon(dimensions.y);
			}

			return dimensions;
		}
	};

	Camera::Camera(AspectRatio aspectRatio)
		: m_impl(new Camera::Impl(aspectRatio))
	{
	}
	Camera::~Camera()
	{
		delete m_impl;
	}
	const glm::mat4& Camera::GetProjectionMatrix()
	{
		return m_impl->m_proj;
	}
	const glm::mat4& Camera::GetViewMatrix()
	{
		return m_impl->m_view;
	}
	const glm::mat4& Camera::GetModelMatrix()
	{
		return m_impl->m_model;
	}
	
	glm::mat4 Camera::GetMVP()
	{
		return m_impl->GetMVP();
	}

	void Camera::SetBackgroundColor(const Color& nColor)
	{
		m_impl->m_background_color = nColor;
	}
	const Color& Camera::GetBackgroundColor()
	{
		return m_impl->m_background_color;
	}

	void Camera::SetAspectRatio(AspectRatio nAspectRatio)
	{
		m_impl->SetAspectRatio(nAspectRatio);
	}
	Vec2f Camera::GetCameraDimensionsFromViewport(float viewportWidth, float viewportHeight)
	{
		return m_impl->GetCameraDimensionsFromViewport(viewportWidth, viewportHeight);
	}
}
