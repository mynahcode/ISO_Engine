#include <IsoEngine.h>

#include "IsoEngine/Platform/OpenGL/OpenGLShader.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class TestLayer : public IE::Layer
{
public:
	TestLayer()
		: Layer("Test"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
		/* Rendering The Triangle -- All temporary test code */
		m_VertexArray.reset(IE::VertexArray::Create());

		/* Populate w/ Vertex Data -- 3D Coordinates */
		float vertices[7 * 3] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,								// Vertex 1 -- Z_coordinate = 0 (0-2), Color (3-6)
			 0.5f, -0.5f, 0.0f, 0.2f, 0.0f, 0.8f, 1.0f,								// Vertex 2 -- Z_coordinate = 0
			 0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f								// Vertex 3 -- Z_coordinate = 0
		};
		IE::Ref<IE::VertexBuffer> m_VertexBuffer;
		m_VertexBuffer.reset(IE::VertexBuffer::Create(vertices, sizeof(vertices)));

		IE::BufferLayout layout = {
				{ IE::ShaderDataType::Float3, "a_Position"},
				{ IE::ShaderDataType::Float4, "a_color"}
		};
		m_VertexBuffer->SetLayout(layout);

		/* Binds vertex Array and Vertex Buffer, sets layout, and links them together */
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		/* Create Element (Index) Buffer */
		IE::Ref<IE::IndexBuffer> m_IndexBuffer;
		unsigned int indices[3] = { 0, 1, 2 };
		m_IndexBuffer.reset(IE::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		///////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////

		/* Rendering the Square */
		m_SquareVertexArray.reset(IE::VertexArray::Create());

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,				// Texture coords: Bottom Left (0.0f, 0.0f)
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,				// Texture coords: Bottom Right (1.0f, 0.0f)
			 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,				// Texture coords: Top Right (1.0f, 1.0f)
			-0.5f, 0.5f, 0.0f, 0.0f, 1.0f				// Texture coords: Top Left (0.0f, 1.0f)
		};

		IE::Ref<IE::VertexBuffer> m_SquareVertexBuffer;
		m_SquareVertexBuffer.reset(IE::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		m_SquareVertexBuffer->SetLayout({
			{ IE::ShaderDataType::Float3, "a_Position"},
			{ IE::ShaderDataType::Float2, "a_TextureCoord"}
			});
		m_SquareVertexArray->AddVertexBuffer(m_SquareVertexBuffer);

		unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		IE::Ref<IE::IndexBuffer> m_SquareIndexBuffer;
		m_SquareIndexBuffer.reset((IE::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t))));
		m_SquareVertexArray->SetIndexBuffer(m_SquareIndexBuffer);

		/* Initializing the Shaders */
		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
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
		m_Shader.reset(IE::Shader::Create(vertexSrc, fragmentSrc));

		/* Square Shader */
		std::string flatColorVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}

		)";

		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			
			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}

		)";
		m_FlatColorShader.reset(IE::Shader::Create(flatColorVertexSrc, flatColorShaderFragmentSrc));	

		/* Texture Shader */
		std::string textureVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TextureCoord;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec2 v_TextureCoord;

			void main()
			{
				v_TextureCoord = a_TextureCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}

		)";

		std::string textureShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec2 v_TextureCoord;
			
			uniform sampler2D u_Texture;

			void main()
			{
				color = texture(u_Texture, v_TextureCoord);
			}

		)";

		m_TextureShader.reset(IE::Shader::Create(textureVertexSrc, textureShaderFragmentSrc));
		m_Texture = IE::Textures2D::Create("assets/textures/grass.jpg");
		m_LogoTexture = IE::Textures2D::Create("assets/textures/ChernoLogo.png");

		std::dynamic_pointer_cast<IE::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<IE::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(IE::Timestep timestep) override
	{
		IE::IELogger::IsoLogger::SetPriority(IE::IELogger::IELogger_Priority::TRACE);
		ISOLOGGER_TRACE("Time step: % seconds (% milliseconds)", timestep.GetSeconds(), timestep.GetMilliseconds());
		/* Basic Camera Movement */
		if (IE::Input::IsKeyPressed(IE_KEY_UP)) m_CameraPosition.y += m_CameraMoveSpeed * timestep;
		else if (IE::Input::IsKeyPressed(IE_KEY_DOWN)) m_CameraPosition.y -= m_CameraMoveSpeed * timestep;
		if (IE::Input::IsKeyPressed(IE_KEY_RIGHT)) m_CameraPosition.x += m_CameraMoveSpeed * timestep;
		else if (IE::Input::IsKeyPressed(IE_KEY_LEFT)) m_CameraPosition.x -= m_CameraMoveSpeed * timestep;
		if (IE::Input::IsKeyPressed(IE_KEY_Q)) m_CameraRotation += m_CameraRotationSpeed * timestep; // Positive rotation 
		else if (IE::Input::IsKeyPressed(IE_KEY_E)) m_CameraRotation -= m_CameraRotationSpeed * timestep; // Negative rotation -- Counterintuitively		

		/* Starts scene and contains all information of scene.*/
		IE::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 }); // Shouldnt render onto clear color.
		IE::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		/* Renderer Flow */
		IE::Renderer::BeginScene(m_Camera); // parameters should be: camera, lights, environment

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		/*
		IE::MaterialRef material = new IE::Material(m_FlatColorShader);
		IE::MaterialInstanceRef material_ir = new IE::MaterialInstanceREf(material);

		material_ir->Set("u_Color", redColor);
		squareMesh->SetMaterial(material_ir);
		*/

		std::dynamic_pointer_cast<IE::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<IE::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				IE::Renderer::Submit(m_FlatColorShader, m_SquareVertexArray, transform); 
			}
		}
		m_Texture->Bind();
		IE::Renderer::Submit(m_TextureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_LogoTexture->Bind();
		IE::Renderer::Submit(m_TextureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		IE::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(IE::Event& ev) override
	{

	}

private:
	IE::Ref<IE::Shader> m_Shader;
	IE::Ref<IE::VertexArray> m_VertexArray;

	IE::Ref<IE::Shader> m_FlatColorShader, m_TextureShader;
	IE::Ref<IE::VertexArray> m_SquareVertexArray;

	IE::Ref<IE::Textures2D> m_Texture, m_LogoTexture;

	IE::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 5.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 180.0f;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
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
