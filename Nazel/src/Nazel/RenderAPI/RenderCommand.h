#pragma once
#include "Nazel/RenderAPI/RenderAPI.h"

namespace Nazel {
class RenderCommand
{
public:
	inline static void Init() { 
		s_RenderAPI->Init();
	}
	inline static void SetClearColor(const glm::vec4& color) {
		s_RenderAPI->SetClearColor(color);
	}
	inline static void Clear() {
		s_RenderAPI->Clear();
	}
	inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) {
		s_RenderAPI->DrawIndexed(vertexArray);
	}
private:
	static RenderAPI* s_RenderAPI;
};
}

