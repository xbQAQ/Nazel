#pragma once

#include "Nazel/Window.h"

#include <glad/glad.h>	//glad 获取一堆 glxxx 函数的函数指针
#include <GLFW/glfw3.h> //glfw 管理操作系统的窗口管理器给到的 framebuffer

struct GLFWwindow;

namespace Nazel {
class WindowsWindow : public Window
{
public:
	WindowsWindow(const WindowProps& props);
	virtual ~WindowsWindow();

	void OnUpdate() override;

	inline unsigned int GetWidth() const override { return m_Data.Width; }
	inline unsigned int GetHeight() const override { return m_Data.Height; }

	// Window attributes
	inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
	void SetVSync(bool enabled) override;
	bool IsVSync() const override;

	void* GetNativeWindow() const override { return m_Window; }
private:
	virtual void Init(const WindowProps& props);
	virtual void Shutdown();
private:
	GLFWwindow* m_Window;
	struct WindowData
	{
		std::string Title;
		unsigned int Width, Height;
		bool VSync;

		EventCallbackFn EventCallback;
	};
	WindowData m_Data;
};
}

