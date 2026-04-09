#include "Window.h"
// #include "glad/glad.h"

#include "Events/Events.h"
#include "Events/KeyboardEvents.h"
#include "Events/MouseEvents.h"

namespace Engine
{

	// TODO: fix hacky solution to find app start time
	auto start = std::chrono::system_clock::now();

	Window::Window(unsigned int width,
				   unsigned int height,
				   const char *title,
				   std::function<void(Event &)> func)
	{
		m_WindowWidth = width;
		m_WindowHeight = height;
		m_WindowTitle = title;
		p_OnEventFunc = std::move(func);

		m_ShouldCloseWindow = false;

		m_Window = glfwCreateWindow(
			m_WindowWidth, m_WindowHeight, m_WindowTitle, nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);

		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		glfwWindowHint(GLFW_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_VERSION_MINOR, 3);

		glfwSwapInterval(0);

		if (!m_Window) {
			std::cout << "FAILED TO CREATE WINDOW\n";
		}

		SetWindowCallbacks();
	}

	Window::Window(const char *title, std::function<void(Event &)> func)
	{
		GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode *mode = glfwGetVideoMode(primaryMonitor);
		m_WindowWidth = mode->width;
		m_WindowHeight = mode->height;
		m_WindowTitle = title;
		p_OnEventFunc = std::move(func);

		m_ShouldCloseWindow = false;

		m_Window = glfwCreateWindow(m_WindowWidth,
									m_WindowHeight,
									m_WindowTitle,
									primaryMonitor,
									nullptr);
		glfwMakeContextCurrent(m_Window);

		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		glfwWindowHint(GLFW_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_VERSION_MINOR, 3);

		glfwSwapInterval(0);

		if (!m_Window) {
			std::cout << "FAILED TO CREATE WINDOW\n";
		}

		SetWindowCallbacks();
	}

	Window::~Window() { glfwDestroyWindow(m_Window); }

	void Window::SetWindowCallbacks()
	{
		// initialize a buffer storing window that is used in the lambda
		// functions
		glfwSetWindowUserPointer(m_Window, this);

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow *window) {
			const Window &win = *(Window *) glfwGetWindowUserPointer(window);
			WindowClosedEvent e(win);
			win.p_OnEventFunc(e);
		});

		glfwSetWindowSizeCallback(
			m_Window, [](GLFWwindow *window, int width, int height) {
				Window &win = *(Window *) glfwGetWindowUserPointer(window);
				WindowResizedEvent e(win, width, height);
				win.p_OnEventFunc(e);
			});

		glfwSetMouseButtonCallback(
			m_Window, [](GLFWwindow *window, int button, int action, int) {
				Window &win = *(Window *) glfwGetWindowUserPointer(window);

				double mouseX, mouseY;
				glfwGetCursorPos(window, &mouseX, &mouseY);

				int width, height;
				glfwGetWindowSize(window, &width, &height);

				if (action == GLFW_PRESS) {
					MouseButtonPressedEvent e(win,
											  button,
											  (float) (mouseX / width),
											  (float) 1 - (mouseY / height));
					win.p_OnEventFunc(e);
				} else if (action == GLFW_RELEASE) {
					MouseButtonReleasedEvent e(win,
											   button,
											   (float) (mouseX / width),
											   (float) 1 - (mouseY / height));
					win.p_OnEventFunc(e);
				}
			});

		glfwSetCursorPosCallback(
			m_Window, [](GLFWwindow *window, double mouseX, double mouseY) {
				Window &win = *(Window *) glfwGetWindowUserPointer(window);

				int width, height;
				glfwGetWindowSize(window, &width, &height);

				MouseMovedEvent e(
					win, mouseX * 2 / width - 1, -mouseY * 2 / height + 1);
				win.p_OnEventFunc(e);
			});

		glfwSetCharCallback(
			m_Window, [](GLFWwindow *window, unsigned int codepoint) {
				Window &win = *(Window *) glfwGetWindowUserPointer(window);

				KeyboardCharTypedEvent e(win, codepoint);
				win.p_OnEventFunc(e);
			});

		glfwSetKeyCallback(
			m_Window,
			[](GLFWwindow *window,
			   int key,
			   int scancode,
			   int action,
			   int mods) {
				Window &win = *(Window *) glfwGetWindowUserPointer(window);

				switch (action) {
				case GLFW_PRESS: {
					KeyboardKeyPressedEvent e(win, key, scancode, mods);
					win.p_OnEventFunc(e);
					break;
				}
				case GLFW_RELEASE: {
					KeyboardKeyReleasedEvent e(win, key, scancode, mods);
					win.p_OnEventFunc(e);
					break;
				}
				case GLFW_REPEAT: {
					KeyboardKeyRepeatedEvent e(win, key, scancode, mods);
					win.p_OnEventFunc(e);
					break;
				}
				default: break;
				}
			});

		glfwSetScrollCallback(
			m_Window, [](GLFWwindow *window, double x, double y) {
				Window &win = *(Window *) glfwGetWindowUserPointer(window);

				MouseScrolledEvent e(win, x, y);
				win.p_OnEventFunc(e);
			});
	}

	void Window::Update() { glfwSwapBuffers(m_Window); }

	bool Window::OnEvent_WindowClosed(WindowClosedEvent &)
	{
		m_ShouldCloseWindow = true;
		return true;
	}

	bool Window::OnEvent_WindowResize(WindowResizedEvent &e)
	{
		m_WindowWidth = e.GetWidth();
		m_WindowHeight = e.GetHeight();
		glViewport(0, 0, m_WindowWidth, m_WindowHeight);
		return true;
	}

	glm::ivec2 Window::GetWindowSize() const
	{ return {m_WindowWidth, m_WindowHeight}; }

	float GetCurTime()
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(
				   std::chrono::system_clock::now() - start)
				   .count() /
			   1000000.f;
	}
}	 // namespace Engine