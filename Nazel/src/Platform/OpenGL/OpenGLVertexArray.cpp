﻿#include "pch.h"
#include "OpenGLVertexArray.h"
#include "glad/glad.h"

namespace Nazel {
static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
	switch (type) {
	case ShaderDataType::Float:   return GL_FLOAT;
	case ShaderDataType::Float2:  return GL_FLOAT;
	case ShaderDataType::Float3:  return GL_FLOAT;
	case ShaderDataType::Float4:  return GL_FLOAT;
	case ShaderDataType::Mat3:    return GL_FLOAT;
	case ShaderDataType::Mat4:    return GL_FLOAT;
	case ShaderDataType::Int:     return GL_INT;
	case ShaderDataType::Int2:    return GL_INT;
	case ShaderDataType::Int3:    return GL_INT;
	case ShaderDataType::Int4:    return GL_INT;
	case ShaderDataType::Bool:    return GL_BOOL;
	}
	NZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
	return 0;
}

OpenGLVertexArray::OpenGLVertexArray() {
	PROFILE_FUNCTION();

	glCreateVertexArrays(1, &m_RendererID);
}
OpenGLVertexArray::~OpenGLVertexArray() {
	PROFILE_FUNCTION();

	glDeleteVertexArrays(1, &m_RendererID);
}
void OpenGLVertexArray::Bind() const {
	PROFILE_FUNCTION();

	glBindVertexArray(m_RendererID);
}
void OpenGLVertexArray::Unbind() const {
	PROFILE_FUNCTION();

	glBindVertexArray(0);
}
void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) {
	PROFILE_FUNCTION();

	NZ_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout");
	glBindVertexArray(m_RendererID);
	vertexBuffer->Bind();

	uint32_t index = 0;
	const auto& layout = vertexBuffer->GetLayout();
	for (const auto& element : layout) {
		glEnableVertexAttribArray(index + m_VertexBufferIndexOffset);
		glVertexAttribPointer(index + m_VertexBufferIndexOffset,
							  element.GetComponentCount(),
							  ShaderDataTypeToOpenGLBaseType(element.Type),
							  element.Normalized,
							  layout.GetStride(),
							  (const void*)element.Offset);
		index++;
	}

	m_VertexBuffers.push_back(vertexBuffer);
	// 修复 OpenGLVertexArray 不能同时绑定多个 VertexBuffer 的问题
	m_VertexBufferIndexOffset += layout.GetElements().size();
}
void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) {
	PROFILE_FUNCTION();

	glBindVertexArray(m_RendererID);
	indexBuffer->Bind();
	m_IndexBuffer = indexBuffer;
}
}