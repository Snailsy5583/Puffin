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

		void Run();

		bool OnWindowClose(WindowCloseEvent& e);

		void OnEvent(Event& e);

		void PushLayer(Layer* layer, bool isOverlay = false);
		void PopLayer(Layer* layer, bool isOverlay = false);


		static Application& GetApp();

		inline float GetDeltaTime() const { return m_DeltaTime; }
	private:
		bool m_Running = true;
		float m_DeltaTime;

		LayerStack m_LayerStack;

		std::unique_ptr<Window> m_Window;

		static Application* s_App;
	};

	Application* CreateApplication();
}

