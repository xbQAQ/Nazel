#include <Nazel.h>

#include "imgui/imgui.h"
#include "Nazel/RenderAPI/Shader.h"
#include "Nazel/RenderAPI/Buffer.h"
#include "Nazel/RenderAPI/VertexArray.h"
#include "Nazel/RenderAPI/OrthographicCamera.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include "Sandbox2D.h"

#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>


// ---Entry Point---------------------
#include "Nazel/Core/EntryPoint.h"
// -----------------------------------


class Sandbox : public Nazel::Application
{
public:
	Sandbox() {
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}
	~Sandbox() {}
};

Nazel::Application* Nazel::CreateApplication()
{
	return new Sandbox();
}


