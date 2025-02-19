#pragma once
#include "RenderCommand.h"

namespace Nazel {
class Renderer
{
public:
	// 将场景、相机、光照等数据传入渲染管线
	static void BeginScene();
	static void EndScene();
	static void Submit(const std::shared_ptr<VertexArray>& vertexArray);
	inline static RenderAPI::API GetAPI() { return RenderAPI::GetAPI(); }
};
}

