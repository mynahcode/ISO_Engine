#include "iepch.h"
#include "RenderCommand.h"

#include "IsoEngine/Platform/OpenGL/OpenGLRendererAPI.h"

namespace IE
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}