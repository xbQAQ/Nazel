#pragma once
#include "RenderCommand.h"
#include "Camera.h"
#include "OrthographicCamera.h"
#include "Shader.h"

namespace Nazel {
class Renderer
{
public:
	// 将场景、相机、光照等数据传入渲染管线
	static void BeginScene(OrthographicCamera& camera);
	static void EndScene();
	static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& modelTransform = glm::mat4(1.0f));
	inline static RenderAPI::API GetAPI() { return RenderAPI::GetAPI(); }

private:
	struct SceneData
	{
		glm::mat4 ProjectionViewMatrix;
	};
	static SceneData* m_SceneData;
};
}

