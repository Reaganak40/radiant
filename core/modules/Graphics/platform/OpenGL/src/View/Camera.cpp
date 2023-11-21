#include "pch.h"
#include "Camera.h"

#define FLOAT_EPSILON 0.0001

/*
	Rounds the floating point number to resolve inprecision in float math.
*/
template<typename T>
T ApplyEpsilon(T d)
{
	{
		if (d > 0) {
			if (abs(d) - abs(floor(d)) < (T)FLOAT_EPSILON) {
				return floor(d);
			}
			else if ((abs(ceil(d)) - abs(d)) < (T)FLOAT_EPSILON) {
				return ceil(d);
			}
		}
		else if (d < 0) {
			if (abs(d) - abs(ceil(d)) < (T)FLOAT_EPSILON) {
				return ceil(d);
			}
			else if ((abs(floor(d)) - abs(d)) < (T)FLOAT_EPSILON) {
				return floor(d);
			}
		}

		return d;
	}
}

namespace rdt::glCore {
	
	struct Camera::Impl {
		glm::mat4 m_proj;
		glm::mat4 m_view;
		glm::mat4 m_model;

		glm::mat4 m_last_MVP;

		Impl()
		{
			m_view = glm::translate(glm::mat4(1.0f), glm::vec3(0,0, 0));
			m_model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
			m_proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
		}

		~Impl()
		{
		}

		glm::mat4* GetMVP()
		{
			m_last_MVP = m_proj * m_view * m_model;
			return &m_last_MVP;
		}

		void SetProjectionMatrix(float width, float height) 
		{
			m_proj = glm::ortho(0.0f, width, 0.0f, height, -1.0f, 1.0f);
		}

		void SetViewMatrix(float translationX, float translationY, float translationZ)
		{
			m_view = glm::translate(glm::mat4(1.0f), glm::vec3(translationX, translationY, translationZ));
		}
	};

	Camera::Camera(AspectRatio aspectRatio)
		: m_impl(new Camera::Impl)
	{
		m_ID = GL_CORE_NULL_CAMERA_ID;
		m_aspectRatio = aspectRatio;
		switch (aspectRatio) {
		case AR_16_9:
			windowWidth = 1920.0f;
			windowHeight = 1080.0f;
			break;
		case AR_1_1:
			windowWidth = 1000.0f;
			windowHeight = 1000.0f;
			break;
		}
		
		pos[0] = 0.0f;
		pos[1] = 0.0f;
		pos[2] = 0.0f;

		float translation[3];
		GetTranslation(&translation[0], &translation[1], &translation[2]);

		m_impl->SetViewMatrix(translation[0], translation[1], translation[2]);
		m_impl->SetProjectionMatrix(windowWidth, windowHeight);
	}

	Camera::~Camera()
	{
		delete m_impl;
	}

	CameraID Camera::GetID()
	{
		return m_ID;
	}
	
	GLM_IMPL_MAT4* Camera::GetMVP()
	{
		return m_impl->GetMVP();
	}

	void Camera::SetAspectRatio(AspectRatio nAspectRatio)
	{
		m_aspectRatio = nAspectRatio;
		switch (nAspectRatio) {
		case AR_16_9:
			windowWidth = 1920.0f;
			windowHeight = 1080.0f;
			break;
		case AR_1_1:
			windowWidth = 1000.0f;
			windowHeight = 1000.0f;
			break;
		}

		m_impl->SetProjectionMatrix(windowWidth, windowHeight);
	}

	void Camera::SetMaintainAspectRatio(bool maintain)
	{
		maintain_aspect_ratio = maintain;
	}

	bool Camera::ShouldMaintainAspectRatio()
	{
		return maintain_aspect_ratio;
	}

	void Camera::FocusOn(float x, float y)
	{
		pos[0] = x - windowWidth / 2;
		pos[1] = y - windowWidth / 2;

		float translation[3];
		GetTranslation(&translation[0], &translation[1], &translation[2]);
		m_impl->SetViewMatrix(translation[0], translation[1], translation[2]);
	}

	void Camera::GetPosition(float* outX, float* outY, float* outZ)
	{
		*outX = pos[0];
		*outY = pos[1];
		*outZ = pos[2];
	}

	void Camera::GetCenter(float* outX, float* outY, float* outZ)
	{
		*outX = pos[0] + windowWidth / 2;
		*outY = pos[1] + windowHeight / 2;
		*outZ = pos[2];
	}

	float Camera::GetCameraWindowWidth()
	{
		return windowWidth;
	}

	float Camera::GetCameraWindowHeight()
	{
		return windowHeight;
	}

	void Camera::GetCameraDimensionsFromViewport(int viewportWidth, int viewportHeight, int* outWidth, int* outHeight)
	{
		float dimensions[2] = { 0.0f, (float)viewportHeight };

		switch (m_aspectRatio) {
		case AR_16_9:
			dimensions[0] = (16.0f / 9.0f) * dimensions[1];
			break;
		case AR_1_1:
			dimensions[0] = dimensions[1];
			break;
		}
		
		if (dimensions[0] > viewportWidth) {
			dimensions[0] = (float)viewportWidth;
			switch (m_aspectRatio) {
			case AR_16_9:
				dimensions[1] = (9 / 16.0f) * dimensions[1];
				break;
			case AR_1_1:
				dimensions[1] = dimensions[0];
				break;
			}
		}

		dimensions[0] = ApplyEpsilon(dimensions[0]);
		dimensions[1] = ApplyEpsilon(dimensions[1]);

		*outWidth  = (int)dimensions[0];
		*outHeight = (int)dimensions[1];
	}

	void Camera::GetTranslation(float* outX, float* outY, float* outZ)
	{
		*outX = -pos[0];
		*outY = -pos[1];
		*outZ = -pos[2];
	}
}
