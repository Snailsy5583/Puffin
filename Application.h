#pragma once

#include <chrono>

#include "Events/Events.h"
#include "Layer.h"
#include "Window.h"

namespace Engine
{


	class Application
	{
	public:
		Application(int width, int height, const char *title);

		Application(const char *title);

		virtual ~Application();

		virtual void Run();

		virtual void PollEvents();

		virtual void Update(float deltaTime);

		virtual void OnEvent(Event &e);

		virtual bool DispatchEvents(EventDispatcher &dispatcher);

	protected:
		Window *m_MainWindow;

		LayerStack m_LayerStack;

		std::chrono::time_point<std::chrono::steady_clock> m_LastFrame;
		float m_DeltaTime;
	};

}	 // namespace Engine