#include "iepch.h"
#include "RenderCommand.h"

#include "IsoEngine/Platform/OpenGL/OpenGLRendererAPI.h"

namespace IE
{
	Scope<RendererAPI> RenderCommand::s_RendererAPI = CreateScope<OpenGLRendererAPI>();
}