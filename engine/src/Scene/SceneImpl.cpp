#include "pch.h"
#include "SceneImpl.hpp"

rdt::scene::SceneImpl::SceneImpl()
{
}

rdt::scene::SceneImpl::~SceneImpl()
{
}

void rdt::scene::SceneImpl::SetSceneName(const char* nName)
{
	m_name = nName;
}

void rdt::scene::SceneImpl::RemoveFromLayerStack(LayerID lID)
{
	size_t index = 0;
	for (auto layer : m_layer_stack) {
		if (layer->GetID() == lID) {
			m_layer_stack.erase(m_layer_stack.begin() + index);
			return;
		}
		index++;
	}

	RDT_CORE_WARN("Scene - LayerID [{}] not found in layer stack (could not remove)", lID);
}

void rdt::scene::SceneImpl::AddToLayerStack(Layer* layer, size_t index)
{
	if (layer == nullptr) {
		return;
	}
	m_layer_stack.insert(m_layer_stack.begin() + index, layer);
}

void rdt::scene::SceneImpl::BindLayers()
{
	for (auto layer : m_layer_stack) {
		layer->Bind();
	}
}

void rdt::scene::SceneImpl::ReleaseLayers()
{
	for (auto layer : m_layer_stack) {
		layer->Release();
	}
}

void rdt::scene::SceneImpl::DetachAll()
{
	if (is_bound) {
		ReleaseLayers();
	}

	for (auto layer : m_layer_stack) {
		layer->Detach();
	}

	m_layer_stack.clear();
}
