#include "pch.h"
#include "SceneImpl.hpp"

#include <Radiant/Scene/Layer.hpp>
#include <Radiant/Logger.hpp>

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
	// the order of the the layer is actually reversed, so the "top"
	// is actually index 0.
	// This is done so when traversing the layer stack during OnUpdate, we can
	// do forward iteration
	index = m_layer_stack.size() - index;
	m_layer_stack.insert(m_layer_stack.begin() + index, layer);
}

void rdt::scene::SceneImpl::BindLayer(const std::string& layerName)
{
	for (auto layer : m_layer_stack) {
		if (layer->GetName() == layerName) {
			layer->Bind();
			return;
		}
	}

	RDT_CORE_WARN("Scene - Could not bind layer '{}', not in layer stack.", layerName);
}

void rdt::scene::SceneImpl::BindLayer(size_t stackIndex)
{
	if (stackIndex >= m_layer_stack.size()) {
		RDT_CORE_WARN("Scene - Could not bind layer at index [{}], out of range.", stackIndex);
		return;
	}

	stackIndex = m_layer_stack.size() - stackIndex;
	m_layer_stack[stackIndex]->Bind();
}

void rdt::scene::SceneImpl::BindLayers()
{
	for (auto layer : m_layer_stack) {
		layer->Bind();
	}
}

void rdt::scene::SceneImpl::ReleaseLayer(const std::string& layerName)
{
	for (auto layer : m_layer_stack) {
		if (layer->GetName() == layerName) {
			layer->Release();
			return;
		}
	}

	RDT_CORE_WARN("Scene - Could not bind layer '{}', not in layer stack.", layerName);
}

void rdt::scene::SceneImpl::ReleaseLayer(size_t stackIndex)
{
	if (stackIndex >= m_layer_stack.size()) {
		RDT_CORE_WARN("Scene - Could not bind layer at index [{}], out of range.", stackIndex);
		return;
	}

	stackIndex = m_layer_stack.size() - stackIndex;
	m_layer_stack[stackIndex]->Release();
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
