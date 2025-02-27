#include "pch.h"
#include "Renderer.h"
#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Nazel {
Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

void Renderer::Init() { 
	RenderCommand::Init();
}

void Renderer::BeginScene(OrthographicCamera& camera) {
	m_SceneData->ProjectionViewMatrix = camera.GetProjectionViewMatrix();
}
void Renderer::EndScene() {
}
void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& modelTransform) {
	shader->Bind();
	std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ProjectionView", m_SceneData->ProjectionViewMatrix);
	std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ModelTransform", modelTransform);
	vertexArray->Bind();
	RenderCommand::DrawIndexed(vertexArray);
}
}