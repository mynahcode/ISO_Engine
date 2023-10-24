#include "iepch.h"

#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace IE
{
	SceneCamera::SceneCamera(uint32_t width, uint32_t height, ProjectionType projType)
		: m_ProjectionType(projType)
	{
		SetViewportSize(width, height);

		switch (m_ProjectionType)
		{
		case ProjectionType::Isometric:
			SetIsometric(m_OrthographicSize, m_OrthographicNear, m_OrthographicFar);
			break;

		case ProjectionType::Orthographic:
			SetOrthographic(m_OrthographicSize, m_OrthographicNear, m_OrthographicFar);
			break;
		case ProjectionType::Perspective:
			SetPerspective(m_PerspectiveFOV, m_PerspectiveNear, m_PerspectiveFar);
			break;
		}
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Orthographic;
		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetPerspective(float verticalFOV, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Perspective;
		m_PerspectiveFOV = verticalFOV;
		m_PerspectiveNear = nearClip;
		m_PerspectiveFar = farClip;
		RecalculateProjection();
	}	
	
	void SceneCamera::SetIsometric(float size, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Isometric;
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
		if (m_ProjectionType == ProjectionType::Perspective)
		{
			m_Projection = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
		}
		
		else if (m_ProjectionType == ProjectionType::Orthographic)
		{
			float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoBottom = -m_OrthographicSize * 0.5f;
			float orthoTop = m_OrthographicSize * 0.5f;

			m_Projection = glm::ortho(orthoLeft, orthoRight,
				orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
		}
		else if (m_ProjectionType == ProjectionType::Isometric)
		{
			float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoBottom = -m_OrthographicSize * 0.5f;
			float orthoTop = m_OrthographicSize * 0.5f;

			m_Projection = glm::ortho(orthoLeft, orthoRight,
				orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);

			float m_hRotation = 30.0f; // Same axis rotations as Proj Zomb
			float m_vRotation = -45.0f;

			glm::mat4 transform = glm::mat4(1.0f);
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

			glm::mat4 dimetricRotation = glm::rotate(glm::mat4(1.0f), glm::radians(m_hRotation), glm::vec3(1.0f, 0.0f, 0.0f)) *
				glm::rotate(glm::mat4(1.0f), glm::radians(m_vRotation), glm::vec3(0.0f, 0.0f, 1.0f));

			m_Projection = m_Projection * dimetricRotation * rotation;
			//m_ViewMatrix = dimetricRotation * rotation * transform;
			//m_Projection = m_Projection * m_ViewMatrix;
		}
	}
}