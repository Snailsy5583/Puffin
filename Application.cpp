#include "glad/glad.h"

#include "Application.h"
#include "Events/WindowEvents.h"

#include <fstream>

namespace Engine
{

	Application::Application(int width, int height, const char *title)
	{
		m_DeltaTime = 1 / 60.f;
		if (!glfwInit())
			std::cout << "GLFW INIT FAILED\n";

		m_MainWindow = new Window(
			width, height, title, BIND_EVENT_FUNC(Application::OnEvent, this));
		// m_MainWindow = std::make_unique<Engine::Window>(
		//  width, height, title, BIND_EVENT_FUNC(Application::OnEvent, this));
		if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
			std::cout << "Failed to initialize GLAD\n";

		glViewport(0, 0, width, height);
	}

	Application::Application(const char *title)
	{
		m_DeltaTime = 1 / 60.f;
		if (!glfwInit())
			std::cout << "GLFW INIT FAILED\n";

		m_MainWindow =
			new Window(title, BIND_EVENT_FUNC(Application::OnEvent, this));
		// m_MainWindow = std::make_unique<Engine::Window>(
		//  width, height, title, BIND_EVENT_FUNC(Application::OnEvent, this));
		if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
			std::cout << "Failed to initialize GLAD\n";

		auto size = m_MainWindow->GetWindowSize();
		glViewport(0, 0, size.x, size.y);
	}

	Application::~Application() { glfwTerminate(); }

	void Application::Run()
	{
		m_LastFrame = std::chrono::steady_clock::now();

		while (!m_MainWindow->GetShouldCloseWindow()) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			PollEvents();
			Update(m_DeltaTime);

			// swap buffers
			m_MainWindow->Update();

			m_DeltaTime =
				(float) std::chrono::duration_cast<std::chrono::microseconds>(
					std::chrono::steady_clock::now() - m_LastFrame)
					.count() /
				1000000.f;

			m_LastFrame = std::chrono::steady_clock::now();
		}
	}
	void Application::PollEvents() { glfwPollEvents(); }

	bool Application::DispatchEvents(EventDispatcher &dispatcher)
	{
		if (dispatcher.Dispatch<WindowClosedEvent>(
				BIND_EVENT_FUNC(Window::OnEvent_WindowClosed, m_MainWindow)))
			return true;
		if (dispatcher.Dispatch<WindowResizedEvent>(
				BIND_EVENT_FUNC(Window::OnEvent_WindowResize, m_MainWindow)))
			return true;
		return false;
	}

	void Application::OnEvent(Event &e)
	{
		EventDispatcher dispatcher(e);

		if (DispatchEvents(dispatcher))
			return;

		m_LayerStack.OnEvent(e);
	}

	void Application::Update(float deltaTime) { }
}	 // namespace Engine