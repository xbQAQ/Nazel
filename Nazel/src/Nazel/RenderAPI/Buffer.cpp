#include "pch.h"
#include "Buffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Nazel {
VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {
	switch (Renderer::GetAPI()) {
	case RenderAPI::API::None:   NZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
	case RenderAPI::API::OpenGL:  return new OpenGLVertexBuffer(vertices, size);
	}
	NZ_CORE_ASSERT(false, "Unknown RendererAPI!");
	return nullptr;
}
IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count) {
	switch (Renderer::GetAPI()) {
	case RenderAPI::API::None:    NZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
	case RenderAPI::API::OpenGL:  return new OpenGLIndexBuffer(indices, count);
	}
	NZ_CORE_ASSERT(false, "Unknown RendererAPI!");
	return nullptr;
}
}
