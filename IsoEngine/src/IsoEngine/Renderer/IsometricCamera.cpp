#include "iepch.h"
#include "IsometricCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace IE
{
	IsometricCamera::IsometricCamera(float left, float right, float bottom, float top)
		: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
	{
		_IE_PROFILER_FUNCTION();
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void IsometricCamera::SetProjection(float left, float right, float bottom, float top)
	{
		_IE_PROFILER_FUNCTION();

		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f); 
							 //glm::scale(glm::mat4(1.0f), glm::vec3(dimetricScaleX, dimetricScaleY, 1.0f));

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void IsometricCamera::RecalcViewMatrix()
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
}