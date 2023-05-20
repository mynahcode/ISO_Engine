#include "iepch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace IE
{
	// Creates matrices
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	// Called either after setting (or changing) m_Position and/or m_Rotation
	void OrthographicCamera::RecalcViewMatrix()
	{
		// transform = translation_matrix(position) * rotation_matrix(rotation)
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position);
		transform = glm::rotate(transform, m_Rotation, glm::vec3(0, 0, 1));

		// Take inverse of the transform
		SetViewMatrix(glm::inverse(transform));

		SetVPMatrix((m_ProjectionMatrix * m_ViewMatrix));
	}
}