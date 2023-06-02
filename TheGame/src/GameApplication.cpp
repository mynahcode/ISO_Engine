#include <IsoEngine.h>
#include <IsoEngine/Core/EntryPoint.h>

#include "IsoEngine/Platform/OpenGL/OpenGLShader.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GameApplication2D.h"

class TestLayer : public IE::Layer
{
public:
	TestLayer()
		: Layer("Test"), m_CameraController(1280.0f / 720.0f, true)
	{

		/* Rendering a Square */
		m_SquareVertexArray = IE::VertexArray::Create();

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
		m_FlatColorShader = IE::Shader::Create("FlatColor", flatColorVertexSrc, flatColorShaderFragmentSrc);



		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
		m_Texture = IE::Textures2D::Create("assets/textures/grass.jpg");
		m_LogoTexture = IE::Textures2D::Create("assets/textures/ChernoLogo.png");

		std::dynamic_pointer_cast<IE::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<IE::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(IE::Timestep timestep) override
	{
		//IE::IELogger::IsoLogger::SetPriority(IE::IELogger::IELogger_Priority::TRACE);
		//ISOLOGGER_TRACE("Time step: % seconds (% milliseconds)", timestep.GetSeconds(), timestep.GetMilliseconds());	

		// Hook-in to the methods needed -- UPDATE
		m_CameraController.OnUpdate(timestep);

		/* RENDER */
		/* Starts scene and contains all information of scene.*/
		IE::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 }); // Shouldnt render onto clear color.
		IE::RenderCommand::Clear();

		/* Renderer Flow */
		IE::Renderer::BeginScene(m_CameraController.GetCamera()); // parameters should be: lights, environment
		//IE::Renderer::BeginScene(m_Scene);
		//IE::Renderer2::BeginScene(m_Scene2D);
		//IE::Renderer2D::DrawQuad();

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

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

		auto textureShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();
		IE::Renderer::Submit(textureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_LogoTexture->Bind();
		IE::Renderer::Submit(textureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

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
		m_CameraController.OnEvent(ev);
	}

private:
	IE::ShaderLibrary m_ShaderLibrary;
	IE::Ref<IE::Shader> m_Shader;
	IE::Ref<IE::VertexArray> m_VertexArray;

	IE::Ref<IE::Shader> m_FlatColorShader;
	IE::Ref<IE::VertexArray> m_SquareVertexArray;

	IE::Ref<IE::Textures2D> m_Texture, m_LogoTexture;

	IE::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};

class TheGame : public IE::Application
{
public:
	TheGame()
		: Application()
	{
		//PushLayer(new TestLayer());
		PushLayer(new TheGame2D());
	}

	~TheGame()
	{

	}
};

IE::Application* IE::CreateApplication()
{
	return new TheGame();
}
