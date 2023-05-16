#include "iepch.h"
#include "LayerStack.h"

namespace IE
{
	/* Layers intended to persist through lifetime of application -- even when popped from stack
	 * such that they can be pushed back onto the LayerStack when needed. 
	 * Layers get pushed on to the first half of the list, overlays get pushed onto the latter
	 * half of the list. Overlays MUST always come AFTER Layers.
	 */
	LayerStack::LayerStack()
	{
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
			delete layer;
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_Layers.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto iter = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (iter != m_Layers.end())
		{
			m_Layers.erase(iter);
			m_LayerInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto iter = std::find(m_Layers.begin(), m_Layers.end(), overlay);
		if (iter != m_Layers.end())
		{
			m_Layers.erase(iter);
		}
	}
}