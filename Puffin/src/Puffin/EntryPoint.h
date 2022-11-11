#pragma once

#ifdef PF_PLATFORM_WINDOWS

extern Puffin::Application* Puffin::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Puffin::CreateApplication();
	app->Run();
	delete app;
}

#endif