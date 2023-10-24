#include "pch.h"
#include "Camera.h"
#include "Utils/Utils.h"

namespace rdt {
	
	struct Camera::Impl {
		glm::mat4 m_proj;
		glm::mat4 m_view;
		glm::mat4 m_model;

		AspectRatio m_aspectRatio;
		Color m_background_color;
		Rect m_window_shape;

		Impl(AspectRatio aspectRatio)
		{
			m_aspectRatio = aspectRatio;
			switch (aspectRatio) {
			case AR_16_9:
				m_window_shape.SetSize({ 1920.0, 1080.0 });
				break;
			case AR_1_1:
				m_window_shape.SetSize({ 1000.0, 1000.0 });
				break;
			}

			m_window_shape.SetPosition({ m_window_shape.GetWidth() / 2, m_window_shape.GetHeight() / 2 });
			Vec2d translation = GetTranslation();

			m_view = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x, translation.y, 0));
			m_model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
			m_proj = glm::ortho(0.0f, (float)m_window_shape.GetWidth(), 0.0f, (float)m_window_shape.GetHeight(), -1.0f, 1.0f);
			
			m_window_shape._DontFlush();
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
			m_aspectRatio = nAspectRatio;
			switch (nAspectRatio) {
			case AR_16_9:
				m_window_shape.SetSize({ 1920.0, 1080.0 });
				break;
			case AR_1_1:
				m_window_shape.SetSize({ 1000.0, 1000.0 });
				break;
			}
			m_proj = glm::ortho(0.0f, (float)m_window_shape.GetWidth(), 0.0f, (float)m_window_shape.GetHeight(), -1.0f, 1.0f);
		}

		Vec2d GetTranslation()
		{
			Vec2d coords = m_window_shape.GetBottomLeftCorner();
			coords.x = -coords.x;
			coords.y = -coords.y;
			return coords;
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

		void FocusOn(const Vec2d& coords)
		{
			m_window_shape.SetPosition(coords);
			Vec2d translation = GetTranslation();
			m_view = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x, translation.y, 0));
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

	void Camera::FocusOn(const Vec2d& coords)
	{
		m_impl->FocusOn(coords);
	}
	Vec2d Camera::GetPosition()
	{
		return m_impl->m_window_shape.GetBottomLeftCorner();
	}

	Vec2d Camera::GetCenter()
	{
		return m_impl->m_window_shape.GetOrigin();
	}

	Vec2f Camera::GetCameraDimensions()
	{
		return { (float)m_impl->m_window_shape.GetWidth(), (float)m_impl->m_window_shape.GetHeight() };
	}
	const Rect& Camera::GetCameraBoundaryBox()
	{
		return m_impl->m_window_shape;
	}
}
