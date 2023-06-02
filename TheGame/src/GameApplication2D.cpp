#include "GameApplication2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "IsoEngine/Platform/OpenGL/OpenGLShader.h"

TheGame2D::TheGame2D()
	: Layer("TheGame2D"), m_CameraController(1280.0f / 720.0f)
{
}

void TheGame2D::OnAttach()
{
	/* Rendering a Square */
	m_SquareVertexArray = IE::VertexArray::Create();

	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};

	IE::Ref<IE::VertexBuffer> m_SquareVertexBuffer;
	m_SquareVertexBuffer.reset(IE::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
	m_SquareVertexBuffer->SetLayout({
		{ IE::ShaderDataType::Float3, "a_Position"},
		});
	m_SquareVertexArray->AddVertexBuffer(m_SquareVertexBuffer);

	unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	IE::Ref<IE::IndexBuffer> m_SquareIndexBuffer;
	m_SquareIndexBuffer.reset(IE::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	m_SquareVertexArray->SetIndexBuffer(m_SquareIndexBuffer);

	
	m_FlatColorShader = IE::Shader::Create("assets/shaders/FlatColor.glsl");
}

void TheGame2D::OnDetach()
{

}

void TheGame2D::OnUpdate(IE::Timestep timestep)
{
	// Hook-in to the methods needed -- UPDATE
	m_CameraController.OnUpdate(timestep);

	/* RENDER */
		/* Starts scene and contains all information of scene.*/
	IE::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 }); // Shouldnt render onto clear color.
	IE::RenderCommand::Clear();

	/* Renderer Flow */
	IE::Renderer::BeginScene(m_CameraController.GetCamera()); // parameters should be: lights, environment

	std::dynamic_pointer_cast<IE::OpenGLShader>(m_FlatColorShader)->Bind();
	std::dynamic_pointer_cast<IE::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

	IE::Renderer::Submit(m_FlatColorShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	IE::Renderer::EndScene();
}

void TheGame2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void TheGame2D::OnEvent(IE::Event& ev)
{
	m_CameraController.OnEvent(ev);
}