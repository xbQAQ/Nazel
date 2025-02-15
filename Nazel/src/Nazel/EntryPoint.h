#pragma once

#ifdef NZ_PLATFORM_WINDOWS

extern Nazel::Application* Nazel::CreateApplication();

int main(int argc, char** argv)
{
	Nazel::Log::Init();

	LOG_CORE_WARN("Core Log");
	int a = 1;
	LOG_EDITOR_ERROR("Editor Log: {0}", a);
	auto app = Nazel::CreateApplication();
	app->Run();
	delete app;
}

#endif