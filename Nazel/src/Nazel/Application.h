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

	// ������Client��
	Application* CreateApplication();
}

