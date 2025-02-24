#include "pch.h"
#include "Shader.h"
#include "RenderAPI.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Nazel {
Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc) {
	switch (Renderer::GetAPI()) {
	case RenderAPI::API::None: NZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
	case RenderAPI::API::OpenGL: return new OpenGLShader(vertexSrc, fragmentSrc);
	}

	NZ_CORE_ASSERT(false, "Unknown RendererAPI!");
	return nullptr;
}
}
