#pragma once

#include "IsoEngine/Renderer/IsometricCamera.h"
#include "IsoEngine/Core/Timestep.h"

#include "IsoEngine/Events/ApplicationEvent.h" // Resize
#include "IsoEngine/Events/MouseEvent.h" // Scroll
#include "IsoEngine/Events/KeyEvent.h"

namespace IE
{
	struct IsometricCameraBounds
	{
		float Left, Right, Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};

	class IsometricCameraController
	{
	public:
		IsometricCameraController(float aspectRatio, bool rotation = false); // Default: aspectratio * 2 units of space regardless of resolution and adjust accordingly to zoom in/zoom out

		void OnUpdate(Timestep ts);
		void OnEvent(Event& ev);

		void OnResize(float width, float height);

		IsometricCamera& GetCamera() { return m_Camera; }
		const IsometricCamera& GetCamera() const { return m_Camera; }

		void SetZoomLevel(float level) { m_ZoomLevel = level; CalculateCameraView(); }
		float GetZoomLevel() const { return m_ZoomLevel; }

	private:
		void CalculateCameraView();

		bool OnMouseScrolled(MouseScrolledEvent& ev);
		bool OnWindowResized(WindowResizeEvent& ev);

		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;

		IsometricCamera m_Camera;
		IsometricCameraBounds m_Bounds;

		bool m_Rotation;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;

		float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 90.0f;
	};
}