#include "iepch.h"

#include "Camera.h"

namespace IE
{
	Camera::Camera(const glm::mat4& projection)
	{
		m_Projection = projection;
	}
}