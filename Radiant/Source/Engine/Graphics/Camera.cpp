#include "pch.h"
#include "Camera.h"

namespace rdt {
	
	struct Camera::Impl {
		glm::mat4 m_proj;
		glm::mat4 m_view;
		glm::mat4 m_model;

		Vec2i m_viewportPos;
		Vec2i m_viewportSize;

		Vec2d m_aspectRatio;
		Vec2d m_worldCoords;

		Color m_background_color;

		Impl(AspectRatio aspectRatio)
		{
			switch (aspectRatio) {
			case AR_16_9:
				m_aspectRatio = { 1920, 1080 };
				break;
			}
			m_worldCoords = { 0, 0 };

			m_view = glm::translate(glm::mat4(1.0f), glm::vec3(-m_worldCoords.x, -m_worldCoords.y, 0));
			m_model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
			m_proj = glm::ortho(0.0f, (float)m_aspectRatio.x, 0.0f, (float)m_aspectRatio.y, -1.0f, 1.0f);
		}

		~Impl()
		{
		}

		glm::mat4 GetMVP()
		{
			return m_proj * m_view * m_model;
		}

		void GetViewport(int* viewportPosX, int* viewportPosY, int* viewportWidth, int* viewportHeight)
		{
			(*viewportPosX) = m_viewportPos.x;
			(*viewportPosY) = m_viewportPos.y;
			(*viewportWidth) = m_viewportSize.x;
			(*viewportHeight) = m_viewportSize.y;
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

	void Camera::SetViewport(const Vec2i& viewportPos, const Vec2i& viewportSize)
	{
		m_impl->m_viewportPos = viewportPos;
		m_impl->m_viewportSize = viewportSize;
	}
	void Camera::GetViewport(int* viewportPosX, int* viewportPosY, int* viewportWidth, int* viewportHeight)
	{
		m_impl->GetViewport(viewportPosX, viewportPosY, viewportWidth, viewportHeight);
	}
	void Camera::SetBackgroundColor(const Color& nColor)
	{
		m_impl->m_background_color = nColor;
	}
	const Color& Camera::GetBackgroundColor()
	{
		return m_impl->m_background_color;
	}
}
