#include "iepch.h"
#include "IsoEngine/Core/IsoMacros.h"
#include "IsoEngine/Controllers/OrthographicCameraController.h"

#include "IsoEngine/Core/Input.h"
#include "IsoEngine/Core/KeyCodes.h"

namespace IE
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
	{

	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		/* Basic Camera Movement */
		if (Input::IsKeyPressed(IE_KEY_W)) 
			m_CameraPosition.y += m_CameraTranslationSpeed * ts;
		else if (IE::Input::IsKeyPressed(IE_KEY_S))
			m_CameraPosition.y -= m_CameraTranslationSpeed * ts;
		if (Input::IsKeyPressed(IE_KEY_D))
			m_CameraPosition.x += m_CameraTranslationSpeed * ts;
		else if (Input::IsKeyPressed(IE_KEY_A))
			m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
		if (m_Rotation)
		{
			if (Input::IsKeyPressed(IE_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * ts; // Positive rotation 
			else if (Input::IsKeyPressed(IE_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * ts; // Negative rotation -- Counterintuitively	

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& ev)
	{
		EventDispatcher dispatcher(ev);
		dispatcher.Dispatch<MouseScrolledEvent>(IE_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(IE_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& ev)
	{
		m_ZoomLevel -= ev.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& ev)
	{
		float yScale = ev.GetHeight() / 720.0f;
		m_AspectRatio = yScale * (float)ev.GetWidth() / ev.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -yScale * m_ZoomLevel, yScale * m_ZoomLevel);

		return false;
	}
}