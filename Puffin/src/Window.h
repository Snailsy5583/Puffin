#pragma once

#include "pfpch.h"

#include "Puffin/Core.h"
#include "Puffin/Events/Event.h"

namespace Puffin
{

	struct WindowProps
	{
		std::string Title;
		int Width;
		int Height;

		WindowProps(const std::string& title="Puffin Engine", int width=1920, int height=1080)
			: Title(title), Width(width), Height(height)
		{}
	};

	class PUFFIN_API Window
	{
	public:
		using EventCallbackFunc = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const =0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFunc& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};

}