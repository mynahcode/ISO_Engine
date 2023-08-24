#pragma once

#include <glm/glm.hpp>

namespace IE
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top); // Parameters: The bounds of an orthographic matrix, using -1 for near and 1 for far for distance.


		void SetProjection(float left, float right, float bottom, float top);

		//void SetIsometricView(const bool& isIsometric) { isometricView = isIsometric; }

		void SetProjectionMatrix(const glm::mat4& projectionMatrix) { m_ProjectionMatrix = projectionMatrix; }
		const glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; }

		void SetViewMatrix(const glm::mat4& viewMatrix) { m_ViewMatrix = viewMatrix; }
		const glm::mat4 GetViewMatrix() const { return m_ViewMatrix; }

		void SetVPMatrix(const glm::mat4& viewProjectionMatrix) { m_ViewProjectionMatrix = viewProjectionMatrix; }
		const glm::mat4 GetVPMatrix() const { return m_ViewProjectionMatrix; }

		void SetPosition(const glm::vec3& position) { m_Position = position; RecalcViewMatrix(); }
		const glm::vec3 GetPosition() const { return m_Position; }

		void SetRotation(const float& rotation) { m_Rotation = rotation; RecalcViewMatrix(); }
		const float GetRotation() const { return m_Rotation; }

	private:
		void RecalcViewMatrix();

		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix; // once per-frame we must recalculate view matrix -> view matrix is the inverse of the transformation matrix of the camera
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		
		bool isometricView = true;

		float m_Rotation = 0.0f;
	};

}