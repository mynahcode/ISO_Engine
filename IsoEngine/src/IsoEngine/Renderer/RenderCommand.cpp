#include "iepch.h"
#include "IsoEngine/Renderer/RenderCommand.h"

#include "IsoEngine/Platform/OpenGL/OpenGLRendererAPI.h"

namespace IE
{
	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}