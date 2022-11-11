#include <Puffin.h>

class Sandbox : public Puffin::Application
{
public:
	Sandbox() {}
	~Sandbox() {}
};

Puffin::Application* Puffin::CreateApplication()
{
	return new Sandbox();
}