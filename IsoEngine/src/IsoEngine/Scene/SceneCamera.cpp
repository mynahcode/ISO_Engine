#include "iepch.h"

#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace IE
{
	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;
		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
		float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
		float orthoBottom = -m_OrthographicSize * 0.5f;
		float orthoTop = m_OrthographicSize * 0.5f;

		m_Projection = glm::ortho(orthoLeft, orthoRight,
			orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);

		float m_hRotation = 35.264f;
		float m_vRotation = -45.0f;

		glm::mat4 transform = glm::mat4(1.0f);
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		glm::mat4 dimetricRotation = glm::rotate(glm::mat4(1.0f), glm::radians(m_hRotation), glm::vec3(1.0f, 0.0f, 0.0f)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_vRotation), glm::vec3(0.0f, 0.0f, 1.0f));

		m_ViewMatrix = dimetricRotation * rotation * transform;

		m_Projection = m_Projection * m_ViewMatrix;
	}
}