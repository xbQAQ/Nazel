﻿#include "pch.h"
#include "OpenGLContext.h"

#include <glad/glad.h>	//glad 获取一堆 glxxx 函数的函数指针
#include <GLFW/glfw3.h>	//glfw 管理操作系统的窗口管理器给到的 framebuffer
#include <gl/GL.h>

namespace Nazel {
OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
	: m_WindowHandle(windowHandle) {
	NZ_CORE_ASSERT(windowHandle, "Window handle is null!")
}
void OpenGLContext::Init() {
	// 将我们窗口的上下文设置为当前线程的主上下文
	glfwMakeContextCurrent(m_WindowHandle);
	// 获取显卡OpenGL函数定义的地址
	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	NZ_CORE_ASSERT(status, "Failed to initialize Glad!");
}
void OpenGLContext::SwapBuffers() {
	glfwSwapBuffers(m_WindowHandle);// 交换缓冲
}
}