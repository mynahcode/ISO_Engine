#pragma once

#include <glm/glm.hpp>

namespace IE
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projection);

		const glm::mat4& GetProjection() const { return m_Projection; }
	private:
		glm::mat4 m_Projection;
	};
}