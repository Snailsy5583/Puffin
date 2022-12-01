#pragma once

#include "Core.h"

#include "LayerStack.h"
#include "Events/Event.h"
#include "Events/WindowEvent.h"
#include "Window.h"

namespace Puffin
{
	class PUFFIN_API Application
	{
	public:
		Application();
		virtual ~Application();

		bool OnWindowClose(WindowCloseEvent& e);

		void OnEvent(Event& e);

		static Application& GetApp();

		void Run();

		inline float GetDeltaTime() const { return m_DeltaTime; }
	private:
		bool m_Running = true;

		float m_DeltaTime;

		std::unique_ptr<Window> m_Window;

		static Application* s_App;
	};

	Application* CreateApplication();
}

