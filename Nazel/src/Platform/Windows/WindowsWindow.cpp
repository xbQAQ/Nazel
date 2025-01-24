#include "pch.h"
#include "WindowsWindow.h"

#include "Nazel/Events/ApplicationEvent.h"
#include "Nazel/Events/MouseEvent.h"
#include "Nazel/Events/KeyEvent.h"


namespace Nazel {
static bool s_GLFWInitialized = false;

Window* Window::Create(const WindowProps& props) {
	return new WindowsWindow(props);
}

WindowsWindow::WindowsWindow(const WindowProps& props) {
	Init(props);
}

WindowsWindow::~WindowsWindow() {
	Shutdown();
}

void WindowsWindow::Init(const WindowProps& props) {
	m_Data.Title = props.Title;
	m_Data.Width = props.Width;
	m_Data.Height = props.Height;

	LOG_CORE_INFO("Create Window {0} ({1}{2})", props.Title, props.Width, props.Height);

	if (!s_GLFWInitialized) {
		int success = glfwInit();
		NZ_CORE_ASSERT(success, "Could not initialize GLFW!");
		glfwSetErrorCallback([](int error, const char* description) -> void {
			LOG_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
		});
		s_GLFWInitialized = true;
	}

	// 创建窗口
	m_Window = glfwCreateWindow(static_cast<int>(m_Data.Width), static_cast<int>(m_Data.Height), m_Data.Title.c_str(), nullptr, nullptr);

	// 设置上下文
	glfwMakeContextCurrent(m_Window);
	// 加载opengl函数
	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);	
	NZ_CORE_ASSERT(status, "Failed to initialize Glad!");
	glfwSetWindowUserPointer(m_Window, &m_Data);
	SetVSync(true);

	// 设置glfw回调函数

	glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
		WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
		data.Width = width;
		data.Height = height;

		WindowResizeEvent event(width, height);
		data.EventCallback(event);
	});

	glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
		WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
		WindowCloseEvent event;
		data.EventCallback(event);
	});

	glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

		switch (action) {
		case GLFW_PRESS:
		{
			KeyPressedEvent event(key, 0);
			data.EventCallback(event);
			break;
		}
		case GLFW_RELEASE:
		{
			KeyReleasedEvent event(key);
			data.EventCallback(event);
			break;
		}
		case GLFW_REPEAT:
		{
			KeyPressedEvent event(key, 1);
			data.EventCallback(event);
			break;
		}
		default:
			break;
		}
	});

	// 输入字符事件
	glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode) {
		WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

		KeyTypedEvent event(keycode);
		data.EventCallback(event);
	});

	glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
		WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

		switch (action) {
		case GLFW_PRESS:
		{
			MouseButtonPressedEvent event(button);
			data.EventCallback(event);
			break;
		}
		case GLFW_RELEASE:
		{
			MouseButtonReleasedEvent event(button);
			data.EventCallback(event);
			break;
		}
		default:
			break;
		}
	});

	glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
		WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

		MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
		data.EventCallback(event);
	});

	glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
		WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

		MouseMovedEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
		data.EventCallback(event);
	});
}

void WindowsWindow::Shutdown() {
	glfwDestroyWindow(m_Window);
}

void WindowsWindow::OnUpdate() {
	glfwPollEvents();
	glfwSwapBuffers(m_Window);
}

void WindowsWindow::SetVSync(bool enabled) {
	if (enabled)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);
	m_Data.VSync = enabled;
}

bool WindowsWindow::IsVSync() const {
	return m_Data.VSync;
}
}
