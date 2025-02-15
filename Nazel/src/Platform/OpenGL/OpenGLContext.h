#pragma once
#include "Nazel/RenderAPI/GraphicsContext.h"

struct GLFWwindow;
namespace Nazel {
class OpenGLContext : public GraphicsContext
{
public:
	OpenGLContext(GLFWwindow* windowHandle);
	virtual void Init() override;
	virtual void SwapBuffers() override;
private:
	GLFWwindow* m_WindowHandle;
};
}

