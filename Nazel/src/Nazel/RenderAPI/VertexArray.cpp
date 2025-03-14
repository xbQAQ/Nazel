#include "pch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Nazel {
Ref<VertexArray> VertexArray::Create() {
	switch (Renderer::GetAPI()) {
	case RenderAPI::API::None: NZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
	case RenderAPI::API::OpenGL: return std::make_shared<OpenGLVertexArray>();
	}

	NZ_CORE_ASSERT(false, "Unknown RendererAPI!");
	return nullptr;
}
}

