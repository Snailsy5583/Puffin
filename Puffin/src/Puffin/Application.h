#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Window.h"

namespace Puffin
{
	class PUFFIN_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	private:
		bool m_Running = true;

		std::unique_ptr<Window> m_Window;
	};

	Application* CreateApplication();
}

