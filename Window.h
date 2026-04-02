#pragma once

// #include <glad/glad.h>
// #include "g
// #include "glad/glad.h"

#include "GLFW/glfw3.h"

#include <glm/glm.hpp>

#include "Events/WindowEvents.h"

#include <chrono>
#include <iostream>

namespace Engine
{

	class Window
	{
	public:
		Window(unsigned int width,
			   unsigned int height,
			   const char *title,
			   std::function<void(Event &)> func);

		Window(const char *title, std::function<void(Event &)> func);

		~Window();

		void SetWindowCallbacks();

		glm::ivec2 GetWindowSize() const;

		void Update();

		inline GLFWwindow *GetGLFWWindow() const { return m_Window; }

		inline std::string GetClipboard() const
		{ return glfwGetClipboardString(m_Window); }

		inline void SetClipboard(std::string str) const
		{ return glfwSetClipboardString(m_Window, str.c_str()); }

		bool OnEvent_WindowClosed(WindowClosedEvent &e);

		bool OnEvent_WindowResize(WindowResizedEvent &e);

		inline bool GetShouldCloseWindow() const { return m_ShouldCloseWindow; }

		std::function<void(Event &)> p_OnEventFunc;

	private:
		GLFWwindow *m_Window;

		unsigned int m_WindowWidth, m_WindowHeight;
		const char *m_WindowTitle;

		bool m_ShouldCloseWindow;
	};

	float GetCurTime();


}	 // namespace Engine
