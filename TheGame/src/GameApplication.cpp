#include <IsoEngine.h>

#include "imgui/imgui.h"

class TestLayer : public IE::Layer
{
public:
	TestLayer()
		: Layer("Test"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
		/* Rendering The Triangle -- All temporary test code */
		m_VertexArray.reset(IE::VertexArray::Create());

		/* Populate w/ Vertex Data -- 3D Coordinates */
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,								// Vertex 1 -- Z_coordinate = 0 (0-2), Color (3-6)
			 0.5f, -0.5f, 0.0f, 0.2f, 0.0f, 0.8f, 1.0f,								// Vertex 2 -- Z_coordinate = 0
			 0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f								// Vertex 3 -- Z_coordinate = 0
		};
		std::shared_ptr<IE::VertexBuffer> m_VertexBuffer;
		m_VertexBuffer.reset(IE::VertexBuffer::Create(vertices, sizeof(vertices)));

		IE::BufferLayout layout = {
				{ IE::ShaderDataType::Float3, "a_Position"},
				{ IE::ShaderDataType::Float4, "a_color"}
		};
		m_VertexBuffer->SetLayout(layout);

		/* Binds vertex Array and Vertex Buffer, sets layout, and links them together */
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		/* Create Element (Index) Buffer */
		std::shared_ptr < IE::IndexBuffer > m_IndexBuffer;
		unsigned int indices[3] = { 0, 1, 2 };
		m_IndexBuffer.reset(IE::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		///////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////

		/* Rendering the Square */
		m_SquareVertexArray.reset(IE::VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f, 0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f
		};

		std::shared_ptr<IE::VertexBuffer> m_SquareVertexBuffer;
		m_SquareVertexBuffer.reset(IE::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		m_SquareVertexBuffer->SetLayout({
			{ IE::ShaderDataType::Float3, "a_Position"}
			});
		m_SquareVertexArray->AddVertexBuffer(m_SquareVertexBuffer);

		unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IE::IndexBuffer> m_SquareIndexBuffer;
		m_SquareIndexBuffer.reset((IE::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t))));
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

		m_Shader.reset(new IE::Shader(vertexSrc, fragmentSrc));

		std::string vertexSrcSquare = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
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

		m_ShaderSquare.reset(new IE::Shader(vertexSrcSquare, fragmentSrcSquare));
	}

	void OnUpdate(IE::Timestep timestep) override
	{
		IE::IELogger::IsoLogger::SetPriority(IE::IELogger::IELogger_Priority::TRACE);
		ISOLOGGER_TRACE("Time step: % seconds (% milliseconds)", timestep.GetSeconds(),timestep.GetMilliseconds());
		/* Basic Camera Movement */
		if (IE::Input::IsKeyPressed(IE_KEY_UP)) m_CameraPosition.y += m_CameraMoveSpeed * timestep;
		else if(IE::Input::IsKeyPressed(IE_KEY_DOWN)) m_CameraPosition.y -= m_CameraMoveSpeed * timestep;
		if(IE::Input::IsKeyPressed(IE_KEY_RIGHT)) m_CameraPosition.x += m_CameraMoveSpeed * timestep;
		else if(IE::Input::IsKeyPressed(IE_KEY_LEFT)) m_CameraPosition.x -= m_CameraMoveSpeed * timestep;
		if (IE::Input::IsKeyPressed(IE_KEY_Q)) m_CameraRotation += m_CameraRotationSpeed * timestep; // Positive rotation 
		else if (IE::Input::IsKeyPressed(IE_KEY_E)) m_CameraRotation -= m_CameraRotationSpeed * timestep; // Negative rotation -- Counterintuitively

		/* Starts scene and contains all information of scene.*/
		IE::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 }); // Shouldnt render onto clear color.
		IE::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		/* Begin Renderering the scene */
		IE::Renderer::BeginScene(m_Camera); // parameters should be: camera, lights, environment

		IE::Renderer::Submit(m_ShaderSquare, m_SquareVertexArray); // Submit mesh/geometry/primitives.
		IE::Renderer::Submit(m_Shader, m_VertexArray); // Submit mesh/geometry/primitives.

		IE::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{

	}

	void OnEvent(IE::Event& ev) override
	{

	}

private:
	std::shared_ptr<IE::Shader> m_Shader;
	std::shared_ptr<IE::VertexArray> m_VertexArray;

	std::shared_ptr<IE::Shader> m_ShaderSquare;
	std::shared_ptr<IE::VertexArray> m_SquareVertexArray;

	IE::OrthographicCamera m_Camera;

	glm::vec3 m_CameraPosition;

	float m_CameraMoveSpeed = 5.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 180.0f;
};

class TheGame : public IE::Application
{
public:
	TheGame()
		: Application()
	{
		PushLayer(new TestLayer());
	}

	~TheGame()
	{

	}
};

IE::Application* IE::CreateApplication()
{
	return new TheGame();
}
