#include "iepch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace IE
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
	{
		_IE_PROFILER_FUNCTION();
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		_IE_PROFILER_FUNCTION();

		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f); 

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalcViewMatrix()
	{
		if (isometricView)
		{
			float m_hRotation = 35.264f;
			float m_vRotation = -45.0f;

			glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position);
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));

			glm::mat4 dimetricRotation = glm::rotate(glm::mat4(1.0f), glm::radians(m_hRotation), glm::vec3(1.0f, 0.0f, 0.0f)) *
				glm::rotate(glm::mat4(1.0f), glm::radians(m_vRotation), glm::vec3(0.0f, 0.0f, 1.0f));

			m_ViewMatrix = dimetricRotation * rotation * transform;

			m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
		}
		else
		{
			// transform = translation_matrix(position) * rotation_matrix(rotation)
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position);
			transform = glm::rotate(transform, glm::radians(m_Rotation), glm::vec3(0, 0, 1));

			// Take inverse of the transform
			SetViewMatrix(glm::inverse(transform));

			SetVPMatrix((m_ProjectionMatrix * m_ViewMatrix));
		}
	}
}