#pragma once

#ifdef NZ_PLATFORM_WINDOWS

extern Nazel::Application* Nazel::CreateApplication();

int main(int argc, char** argv)
{
	Nazel::Log::Init();

	PROFILE_BEGIN_SESSION("Startup", "Profile-Startup.json");
	auto app = Nazel::CreateApplication();
	PROFILE_END_SESSION();

	PROFILE_BEGIN_SESSION("Runtime", "Profile-Runtime.json");
	app->Run();
	PROFILE_END_SESSION();

	PROFILE_BEGIN_SESSION("Startup", "Profile-Shutdown.json");
	delete app;
	PROFILE_END_SESSION();
}

#endif