#include "pfpch.h"

#include <chrono>
#include "Application.h"
#include "Log.h"

#include <GLAD/glad.h>

namespace Puffin
{

	Application* Application::s_App;

#define BIND_EVENT_FUNC(x) std::bind(&x, this, std::placeholders::_1)

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FUNC(Application::OnEvent));

		s_App = this;
	}

	Application::~Application()
	{}

	void Application::Run()
	{
		while (m_Running)
		{
			m_Window->OnUpdate();
			m_LayerStack.OnUpdate();

			m_DeltaTime = 2;
		}
	}

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
		
		m_LayerStack.OnEvent(e);
	}

	void Application::PushLayer(Layer* layer, bool isOverlay /*= false*/)
	{
		m_LayerStack.Push(layer, isOverlay);
	}

	void Application::PopLayer(Layer* layer, bool isOverlay /*= false*/)
	{
		m_LayerStack.Pop(layer, isOverlay);
	}

	Puffin::Application& Application::GetApp()
	{
		return *s_App;
	}


}