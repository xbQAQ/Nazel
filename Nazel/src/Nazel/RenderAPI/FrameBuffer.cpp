#include "pch.h"
#include "FrameBuffer.h"
#include "Nazel/RenderAPI/Renderer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace Nazel {
Ref<FrameBuffer> Nazel::FrameBuffer::Create(const FramebufferSpecification& spec) {
	switch (Renderer::GetAPI()) {
	case RenderAPI::API::None: NZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
	case RenderAPI::API::OpenGL: return std::make_shared<OpenGLFrameBuffer>(spec);
	}

	NZ_CORE_ASSERT(false, "Unknown RendererAPI!");
	return nullptr;
}
}
