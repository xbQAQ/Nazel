#pragma once

#ifdef NZ_PLATFORM_WINDOWS

extern Nazel::Application* Nazel::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Nazel::CreateApplication();
	app->Run();
	delete app;
}

#endif