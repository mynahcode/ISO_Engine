#include "iepch.h"
#include "IsoEngine/IsoMacros.h"
#include "IsoEngine/Application.h"

#include "IsoEngine/Renderer/Renderer.h"

#include "input.h"
#include "IsoEngine/IsoLogger/IsoLogger.h"

namespace IE 
{
	/* Application is implemented as a Singleton. */

	Application* Application::s_Instance = nullptr;

	Application::Application()
		: m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		/* Create GLFW Window */
		//IE_CORE_ASSERT(!s_Instance, "Application already created!");
		s_Instance = this; // Singleton gets set when we construct the IsoEngine Application and should only be one.

		m_Window = std::unique_ptr<Window>(Window::Create());		// explicit constructor
		m_Window->SetEventCallback(IE_BIND_EVENT_FN(Application::OnEvent));
		m_Window->SetVSync(true);

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		/* Rendering The Triangle -- All temporary test code */
		m_VertexArray.reset(VertexArray::Create());

		/* Populate w/ Vertex Data -- 3D Coordinates */
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,								// Vertex 1 -- Z_coordinate = 0 (0-2), Color (3-6)
			 0.5f, -0.5f, 0.0f, 0.2f, 0.0f, 0.8f, 1.0f,								// Vertex 2 -- Z_coordinate = 0
			 0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f								// Vertex 3 -- Z_coordinate = 0
		};
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		BufferLayout layout = {
				{ ShaderDataType::Float3, "a_Position"},
				{ ShaderDataType::Float4, "a_color"}
		};
		m_VertexBuffer->SetLayout(layout);

		/* Binds vertex Array and Vertex Buffer, sets layout, and links them together */
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		/* Create Element (Index) Buffer */
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		unsigned int indices[3] = { 0, 1, 2 };
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		///////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////

		/* Rendering the Square */
		m_SquareVertexArray.reset(VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f, 0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f
		};

		std::shared_ptr<VertexBuffer> m_SquareVertexBuffer;
		m_SquareVertexBuffer.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		m_SquareVertexBuffer->SetLayout( {
			{ ShaderDataType::Float3, "a_Position"}
		});
		m_SquareVertexArray->AddVertexBuffer(m_SquareVertexBuffer);

		unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0};
		std::shared_ptr<IndexBuffer> m_SquareIndexBuffer;
		m_SquareIndexBuffer.reset((IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t))));

		m_SquareVertexArray->SetIndexBuffer(m_SquareIndexBuffer);

		/* Initializing the Shaders */
		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}

		)";
		// ^ in DX, we multiply vec4 * u_ViewProjection likely put into a multiply function

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position* 0.5 + 0.5, 1.0);
				color = v_Color;
			}

		)";

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));

		std::string vertexSrcSquare = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);
			}

		)";

		std::string fragmentSrcSquare = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}

		)";

		m_ShaderSquare.reset(new Shader(vertexSrcSquare, fragmentSrcSquare));

	}

	void Application::Run()
	{
		while (m_IsRunning)
		{
			// TODO: Move below code to TheGame project GameApplication.cpp
			/* Starts scene and contains all information of scene.*/
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 }); // Shouldnt render onto clear color.
			RenderCommand::Clear();

			m_Camera.SetPosition({ 0.5f, 0.5f, 0.0f });
			m_Camera.SetRotation(45.0f);

			/* Begin Renderering the scene */
			Renderer::BeginScene(m_Camera); // parameters should be: camera, lights, environment

			Renderer::Submit(m_ShaderSquare, m_SquareVertexArray); // Submit mesh/geometry/primitives.
			Renderer::Submit(m_Shader, m_VertexArray); // Submit mesh/geometry/primitives.

			Renderer::EndScene();

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			/* imgui debug layer */
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();

			//auto [x, y] = Input::GetMousePos(); // DEBUG
			//ISOLOGGER_INFO("<%, %>", x, y);
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