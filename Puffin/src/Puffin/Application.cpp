#include "pfpch.h"

#include "Application.h"

#include "Log.h"
#include "Events/WindowEvent.h"

namespace Puffin
{

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{}

	void Application::Run()
	{
		WindowResizeEvent event(1920, 1080);
		PF_TRACE(event);
		while (m_Running)
		{
			m_Window->OnUpdate();
		}
	}

}