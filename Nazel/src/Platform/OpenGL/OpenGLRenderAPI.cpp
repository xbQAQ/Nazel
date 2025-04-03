#include "pch.h"
#include "OpenGLRenderAPI.h"
#include <glad/glad.h>

namespace Nazel {
void OpenGLRenderAPI::Init() {
	PROFILE_FUNCTION();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
}
void OpenGLRenderAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
	glViewport(x, y, width, height);
}
void OpenGLRenderAPI::SetClearColor(const glm::vec4& color) {
	glClearColor(color.r, color.g, color.b, color.a);
}
void OpenGLRenderAPI::Clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void OpenGLRenderAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount) {
	glDrawElements(GL_TRIANGLES, indexCount == 0 ? vertexArray->GetIndexBuffer()->GetCount() : indexCount, GL_UNSIGNED_INT, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);
}
}
