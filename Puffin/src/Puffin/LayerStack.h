#pragma once

#include <vector>
#include "Layer.h"

namespace Puffin
{

	class PUFFIN_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void Push(Layer* layer, bool isOverlay = false);
		void Pop(Layer* layer, bool isOverlay = false);

		void OnEvent(Event& e);
		void OnUpdate();

		inline std::vector<Layer*>::iterator begin() { return m_Stack.begin(); }
		inline std::vector<Layer*>::iterator end() { return m_Stack.end(); }
	private:
		std::vector<Layer*> m_Stack;
		std::vector<Layer*>::iterator m_LayerInsertIndex;
	};

}