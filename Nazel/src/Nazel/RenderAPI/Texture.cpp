#include "pch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Nazel {

Ref<Texture2D> Texture2D::Create(const std::string& path) {
	switch (Renderer::GetAPI()) {
	case RenderAPI::API::None:    NZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
	case RenderAPI::API::OpenGL:  return std::make_shared<OpenGLTexture2D>(path);
	}

	NZ_CORE_ASSERT(false, "Unknown RendererAPI!");
	return nullptr;
}

}