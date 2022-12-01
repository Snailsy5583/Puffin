#include "pfpch.h"

#include <chrono>
#include "Application.h"
#include "Log.h"

namespace Puffin
{

#define BIND_EVENT_FUNC(x) std::bind(&x, this, std::placeholders::_1)

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FUNC(Application::OnEvent));

		s_App = this;
	}

	Application::~Application()
	{}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	void Application::OnEvent(Event& e)
	{
		PF_TRACE("{0}", e);
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNC(Application::OnWindowClose));
	}

	Puffin::Application& Application::GetApp()
	{
		return *s_App;
	}

	void Application::Run()
	{
		while (m_Running)
		{
			std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
			m_Window->OnUpdate();
			std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

			m_DeltaTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000000.f;
		}
	}

}