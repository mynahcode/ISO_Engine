#include "iepch.h"
#include "IsoEngine/IsoMacros.h"
#include "IsoEngine/Application.h"
#include "input.h"
#include "IsoEngine/IsoLogger/IsoLogger.h"

#include <glad/glad.h>

#define const int GL_COLOR_BUFFER_BIT = 0x00004000;

namespace IE 
{
	/* Application is implemented as a Singleton. */

	Application* Application::s_Instance = nullptr;

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch(type)
		{
			case IE::ShaderDataType::Float:		return GL_FLOAT; 
			case IE::ShaderDataType::Float2:	return GL_FLOAT;
			case IE::ShaderDataType::Float3:	return GL_FLOAT;
			case IE::ShaderDataType::Float4:	return GL_FLOAT;
			case IE::ShaderDataType::Mat3:		return GL_FLOAT;
			case IE::ShaderDataType::Mat4:		return GL_FLOAT;
			case IE::ShaderDataType::Int:		return GL_INT;
			case IE::ShaderDataType::Int2:		return GL_INT;
			case IE::ShaderDataType::Int3:		return GL_INT;
			case IE::ShaderDataType::Int4:		return GL_INT;
			case IE::ShaderDataType::Bool:		return GL_BOOL;
		}
	}

	Application::Application()
	{
		/* Create Window */
		//IE_CORE_ASSERT(!s_Instance, "Application already created!");
		s_Instance = this; // Singleton gets set when we construct the IsoEngine Application and should only be one.

		m_Window = std::unique_ptr<Window>(Window::Create());		// explicit constructor
		m_Window->SetEventCallback(IE_BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		/* Create OpenGL Context */
		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		/* Populate w/ Vertex Data -- 3D Coordinates */
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,								// Vertex 1 -- Z_coordinate = 0 (0-2), Color (3-6)
			0.5f, -0.5f, 0.0f, 0.2f, 0.0f, 0.8f, 1.0f,								// Vertex 2 -- Z_coordinate = 0
			0.0f, 0.0f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f								// Vertex 3 -- Z_coordinate = 0
		};

		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		{
			BufferLayout layout = {
				{ ShaderDataType::Float3, "a_Position"},				// 3-Component Float
				{ ShaderDataType::Float4, "a_color"}
			};

			m_VertexBuffer->SetLayout(layout);
		}

		/* Iterate through every element in layout */
		uint32_t vb_Index = 0;
		auto& layout = m_VertexBuffer->GetLayout();
		for (auto& element : layout)
		{
			/* Layout of Buffer */
			glEnableVertexAttribArray(vb_Index);
			glVertexAttribPointer(vb_Index, 
				element.GetComponentCount(), 
				ShaderDataTypeToOpenGLBaseType(element.Type), 
				element.IsNormalized ? GL_TRUE : GL_FALSE, 
				layout.GetStride(), 
				(void*)element.Offset);

			vb_Index++;
		}

		/* Create Element (Index) Buffer */
		unsigned int indices[3] = { 0, 1, 2 };
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices)/sizeof(uint32_t)));

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);
			}

		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position* 0.5 + 0.5, 1.0);
				color = v_color;
			}

		)";

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));

	}

	void Application::Run()
	{

		glClearColor(0.1f, 0.1f, 0.1f, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		m_Shader->Bind();
		glBindVertexArray(m_VertexArray);
		glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

		while (m_IsRunning)
		{
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			//auto [x, y] = Input::GetMousePos();
			//ISOLOGGER_INFO("<%, %>", x, y);

			m_ImGuiLayer->Begin();

			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();

			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& ev)
	{

		//ISOLOGGER_INFO("%", ev);
		EventDispatcher dispatcher(ev);
		dispatcher.Dispatch<WindowCloseEvent>(IE_BIND_EVENT_FN(Application::OnWindowClose));

		for (auto iter = m_LayerStack.end(); iter != m_LayerStack.begin(); )
		{
			(*--iter)->OnEvent(ev);
			if (ev.m_Handled)
				break;
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& ev)
	{
		m_IsRunning = false;
		return true;
	}
}