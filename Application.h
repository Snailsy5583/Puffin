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
		Application(unsigned int width, unsigned int height, const char *title);

		Application(const char *title);

		~Application();

		virtual void Run();

		virtual void Update(float deltaTime);

		virtual void OnEvent(Engine::Event &e);

		virtual bool DispatchEvents(Engine::EventDispatcher &dispatcher);

	protected:
		Window *m_MainWindow;

		LayerStack m_LayerStack;

		std::chrono::time_point<std::chrono::steady_clock> m_LastFrame;
		float m_DeltaTime;
	};

}	 // namespace Engine