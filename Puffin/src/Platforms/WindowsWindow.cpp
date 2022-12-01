#include "pfpch.h"

#include "WindowsWindow.h"

#include "Puffin/Events/AppEvent.h"
#include "Puffin/Events/KeyEvent.h"
#include "Puffin/Events/MouseEvent.h"
#include "Puffin/Events/WindowEvent.h"


namespace Puffin
{

	static bool s_GLFWInitialized = false;

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		if (!s_GLFWInitialized)
		{
			s_GLFWInitialized = glfwInit();
		}

		m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), NULL, NULL);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);

        SetVSync(false);

		//GLFW Callbacks
 		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
 			{
 				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
 				WindowCloseEvent e;
 				data.EventCallback(e);
 			});

 		glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int focused) 
 			{
 				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
 				if (focused == GLFW_TRUE)
 				{
 					WindowFocusEvent e;
 					data.EventCallback(e);
 				}
 				else
 				{
 					WindowLostFocusEvent e;
 					data.EventCallback(e);
 				}
 			});

 		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
 			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
 				WindowResizeEvent e(width, height);
 				data.EventCallback(e);
			});
 
 		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
 			{
 				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
 
 				switch (action)
 				{
 					case GLFW_PRESS:
 					{
 						KeyPressedEvent e(key, false);
 						data.EventCallback(e);
 						break;
 					}
 					case GLFW_REPEAT:
 					{
 						KeyPressedEvent e(key, true);
 						data.EventCallback(e);
 						break;
 					}
 					case GLFW_RELEASE:
 					{
 						KeyReleasedEvent e(key);
 						data.EventCallback(e);
 						break;
 					}
 				}
 			});
 
 		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x_pos, double y_pos)
 			{
 				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
 
 				MouseMovedEvent e((float)x_pos, (float)y_pos);
 				data.EventCallback(e);
 			});
 
 		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
 			{
 				WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);
 
 				switch (action)
 				{
 				    case GLFW_PRESS:
 				    {
 					    MouseButtonPressedEvent e(button);
 					    data.EventCallback(e);
 					    break;
 				    }
 				    case GLFW_RELEASE:
 				    {
 					    MouseButtonReleasedEvent e(button);
 					    data.EventCallback(e);
 					    break;
 				    }
 				}
 			});
        
        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double x, double y)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                MouseScrolledEvent e(y);
                data.EventCallback(e);
            });
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		glfwSwapInterval(enabled);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

}