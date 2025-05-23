#pragma once
#include "Nazel/RenderAPI/Buffer.h"
#include <cstdint>

namespace Nazel {
class OpenGLVertexBuffer : public VertexBuffer
{
public:
	OpenGLVertexBuffer(uint32_t size);
	OpenGLVertexBuffer(float* vertices, uint32_t size);
	virtual ~OpenGLVertexBuffer();
	virtual void Bind() const override;
	virtual void Unbind() const override;

	void SetData(const void* data, uint32_t size) override;

	virtual const BufferLayout& GetLayout() const override;
	virtual void SetLayout(const BufferLayout& layout) override;
private:
	uint32_t m_RendererID;
	BufferLayout m_Layout;
};

class OpenGLIndexBuffer : public IndexBuffer
{
public:
	OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
	virtual ~OpenGLIndexBuffer();
	virtual void Bind() const override;
	virtual void Unbind() const override;
	virtual uint32_t GetCount() const override;
private:
	uint32_t m_RendererID;
	uint32_t m_Count;
};
}

