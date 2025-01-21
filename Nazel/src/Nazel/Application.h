#pragma once
#include "Core.h"
#include "Events/Event.h"

namespace Nazel {
	class NAZEL_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
	};

	// 定义在Client中
	Application* CreateApplication();
}

