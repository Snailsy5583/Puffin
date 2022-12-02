#pragma once

#include "Core.h"
#include "Events/Event.h"

namespace Puffin
{

	class PUFFIN_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();
		
		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnEvent(Event& e) = 0;

		inline const std::string& GetName() const { return m_Name; }

	protected:
		const std::string& m_Name;
	};

}