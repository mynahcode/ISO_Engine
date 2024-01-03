#include "iepch.h"
#include "IsoEngine/Core/IsoMacros.h"
#include "IsoEngine/Controllers/OrthographicCameraController.h"

#include "IsoEngine/Core/Input.h"
#include "IsoEngine/Core/KeyCodes.h"

namespace IE
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
	{
	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		_IE_PROFILER_FUNCTION();

		/* Basic Camera Movement */
		if (Input::IsKeyPressed(Key::W))
			m_CameraPosition.y += m_CameraTranslationSpeed * ts;
		else if (IE::Input::IsKeyPressed(Key::S))
			m_CameraPosition.y -= m_CameraTranslationSpeed * ts;
		if (Input::IsKeyPressed(Key::D))
			m_CameraPosition.x += m_CameraTranslationSpeed * ts;
		else if (Input::IsKeyPressed(Key::A))
			m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
		if (m_Rotation)
		{
			if (Input::IsKeyPressed(Key::Q))
				m_CameraRotation += m_CameraRotationSpeed * ts; // Positive rotation 
			else if (Input::IsKeyPressed(Key::E))
				m_CameraRotation -= m_CameraRotationSpeed * ts; // Negative rotation -- Counterintuitively	

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& ev)
	{
		_IE_PROFILER_FUNCTION();

		EventDispatcher dispatcher(ev);
		dispatcher.Dispatch<MouseScrolledEvent>(IE_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(IE_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	void OrthographicCameraController::CalculateCameraView()
	{
		_IE_PROFILER_FUNCTION();

		m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);

	}

	void OrthographicCameraController::OnResize(float width, float height)
	{
		_IE_PROFILER_FUNCTION();
		float yScale = height / 2160.0f;
		m_AspectRatio = yScale * width / height;
		CalculateCameraView();
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& ev)
	{
		_IE_PROFILER_FUNCTION();

		m_ZoomLevel -= ev.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		CalculateCameraView();
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& ev)
	{
		_IE_PROFILER_FUNCTION();

		OnResize((float)ev.GetWidth(), (float)ev.GetHeight());
		return false;
	}
}