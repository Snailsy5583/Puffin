#include "pfpch.h"

#include "LayerStack.h"
#include "Events/Event.h"

namespace Puffin
{

	LayerStack::LayerStack()
	{}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Stack)
		{
			delete layer;
		}
	}

	void LayerStack::Push(Layer* layer, bool isOverlay /*= false*/)
	{
		if (!isOverlay)
		{
			m_Stack.emplace(m_LayerInsertIndex, layer);
			m_LayerInsertIndex++;
		}
		else
			m_Stack.emplace_back(layer);

		layer->OnAttach();
	}

	void LayerStack::Pop(Layer* layer, bool isOverlay /*= false*/)
	{
		auto place = std::find(m_Stack.begin(), m_Stack.end(), layer);
		if (place != m_Stack.end())
		{
			m_Stack.erase(place);
			if (!isOverlay)
				m_LayerInsertIndex--;

			layer->OnDetach();
		}
	}

	void LayerStack::OnEvent(Event& e)
	{
		for (auto it = end(); it != begin();)
		{
			it--;
			(* it)->OnEvent(e);

			if (e.GetHandled())
				break;
		}
	}

	void LayerStack::OnUpdate()
	{
		for (Layer* layer : m_Stack)
		{
			layer->OnUpdate();
		}
	}

}