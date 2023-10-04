#include "pch.h"
#include "Camera.h"

namespace rdt::core {
	

	Camera::Camera(AspectRatio aspectRatio)
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
	Camera::~Camera()
	{
	}
	const glm::mat4& Camera::GetProjectionMatrix()
	{
		return m_proj;
	}
	const glm::mat4& Camera::GetViewMatrix()
	{
		return m_view;
	}
	const glm::mat4& Camera::GetModelMatrix()
	{
		return m_model;
	}
	
	glm::mat4 Camera::GetMVP()
	{
		return m_proj * m_view * m_model;
	}

	void Camera::SetViewport(const Vec2d& viewportPos, const Vec2d& viewportSize)
	{
		m_viewportPos = viewportPos;
		m_viewportSize = viewportSize;
	}
	void Camera::GetViewport(int* viewportPosX, int* viewportPosY, int* viewportWidth, int* viewportHeight)
	{
		(*viewportPosX) = m_viewportPos.x;
		(*viewportPosY) = m_viewportPos.y;
		(*viewportWidth) = m_viewportSize.x;
		(*viewportHeight) = m_viewportSize.y;
	}
}
