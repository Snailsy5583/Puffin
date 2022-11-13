#pragma once

#ifdef PF_PLATFORM_WINDOWS

extern Puffin::Application* Puffin::CreateApplication();

int main(int argc, char** argv)
{
	Puffin::Log::Init();
	PF_CORE_WARN("Initialized Log!");
	auto a = 5;
	PF_INFO("This is a client! {0}", 1, a);

	auto app = Puffin::CreateApplication();
	app->Run();
	delete app;
}

#endif