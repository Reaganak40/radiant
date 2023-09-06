#include "pch.h"
#include "RenderCache.h"

namespace Radiant {
	RenderCache::RenderCache()
		: m_rect_index(0), m_line_index(0)
	{
	}
	RenderCache::~RenderCache()
	{
	}
	Mesh& RenderCache::GetMesh(UniqueID meshIdentifier)
	{
		if (m_mesh_cache.find(meshIdentifier) == m_mesh_cache.end()) {
			m_mesh_cache[meshIdentifier] = Mesh();
		}

		return m_mesh_cache.at(meshIdentifier);
	}

	std::shared_ptr<Rect> RenderCache::GetFreeRect()
	{
		if (m_rect_index == m_rect_cache.size()) {
			return std::shared_ptr<Rect>();
		}

		return m_rect_cache.at(m_rect_index++);
	}

	void RenderCache::AddRectToCache(std::shared_ptr<Rect> rect)
	{
		m_rect_cache.push_back(rect);
	}

	void RenderCache::Flush(const UniqueID meshIdentifier)
	{
		m_mesh_cache.erase(meshIdentifier);
	}

	void RenderCache::OnEndFrame()
	{
		m_rect_cache.resize(m_rect_index);
		m_line_cache.resize(m_line_index);

		m_rect_index = 0;
		m_line_index = 0;
	}
}
