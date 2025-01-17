#pragma once
#include "Core.h"

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

