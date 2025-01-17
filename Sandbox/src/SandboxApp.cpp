#include <Nazel.h>

class Sandbox : public Nazel::Application
{
public:
	Sandbox() {}
	~Sandbox() {}
};

Nazel::Application* Nazel::CreateApplication()
{
	return new Sandbox();
}


