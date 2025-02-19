#include "pch.h"
#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRenderAPI.h"

namespace Nazel {
RenderAPI* RenderCommand::s_RenderAPI = new OpenGLRenderAPI;
}
