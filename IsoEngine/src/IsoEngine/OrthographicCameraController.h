#pragma once

#include "IsoEngine/Renderer/OrthographicCamera.h"
#include "IsoEngine/Core/Timestep.h"

#include "IsoEngine/Events/ApplicationEvent.h" // Resize
#include "IsoEngine/Events/MouseEvent.h" // Scroll
#include "IsoEngine/Events/KeyEvent.h"

namespace IE
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false); // Default: aspectratio * 2 units of space regardless of resolution and adjust accordingly to zoom in/zoom out

		void OnUpdate(Timestep ts);
		void OnEvent(Event& ev);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		void SetZoomLevel(float level) { m_ZoomLevel = level; }
		float GetZoomLevel() const { return m_ZoomLevel; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& ev);
		bool OnWindowResized(WindowResizeEvent& ev);

		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		bool m_Rotation;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;

		float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 90.0f;
	};
}