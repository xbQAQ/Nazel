#include "pch.h"
#include "LayerStack.h"


namespace Nazel {
LayerStack::LayerStack() {
}

LayerStack::~LayerStack() {
	for (auto* layer : m_Layers) {
		delete layer;
	}
}

void LayerStack::PushLayer(Layer* layer) {
	m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
	m_LayerInsertIndex++;
	layer->OnAttach();
}

void LayerStack::PushOverlay(Layer* overlay) {
	m_Layers.emplace_back(overlay);
	overlay->OnAttach();
}

void LayerStack::PopLayer(Layer* layer) {
	// 搜索区域分为两部分，任何覆盖层都将被推到顶部，任何（其他）层将插入中间（在插入的层的顶部）。
	auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
	if (it != m_Layers.end()) {
		layer->OnDetach();
		m_Layers.erase(it);
		m_LayerInsertIndex--;
	}
}

void LayerStack::PopOverlay(Layer* overlay) {
	auto it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), overlay);
	if (it != m_Layers.end())
		overlay->OnDetach();
		m_Layers.erase(it);
}

}
