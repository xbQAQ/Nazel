#include "pch.h"
#include "Buffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Nazel {
Ref<VertexBuffer> VertexBuffer::Create(uint32_t size) {
	switch (Renderer::GetAPI()) {
	case RenderAPI::API::None:   NZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
	case RenderAPI::API::OpenGL:  return CreateRef<OpenGLVertexBuffer>(size);
	}
	NZ_CORE_ASSERT(false, "Unknown RendererAPI!");
	return nullptr;
}
Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size) {
	switch (Renderer::GetAPI()) {
	case RenderAPI::API::None:   NZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
	case RenderAPI::API::OpenGL:  return CreateRef<OpenGLVertexBuffer>(vertices, size);
	}
	NZ_CORE_ASSERT(false, "Unknown RendererAPI!");
	return nullptr;
}
Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count) {
	switch (Renderer::GetAPI()) {
	case RenderAPI::API::None:    NZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
	case RenderAPI::API::OpenGL:  return CreateRef<OpenGLIndexBuffer>(indices, count);
	}
	NZ_CORE_ASSERT(false, "Unknown RendererAPI!");
	return nullptr;
}
}
