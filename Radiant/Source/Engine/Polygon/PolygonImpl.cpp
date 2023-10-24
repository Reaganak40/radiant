#include "pch.h"
#include "PolygonImpl.h"
#include "Graphics/Renderer.h"

rdt::core::PolygonImpl::PolygonImpl()
	: m_UUID(GetUniqueID()), m_width(0), m_height(0), m_properties(0), m_rotation(0)
{
	shouldFlush = true;
}

rdt::core::PolygonImpl::~PolygonImpl()
{
	FreeUniqueID(m_UUID);

	if (shouldFlush) {
		Renderer::_FlushPolygon(m_UUID);
	}
}

void rdt::core::PolygonImpl::Copy(const PolygonImpl* src)
{
	m_rotation = src->m_rotation;
	m_properties = src->m_properties;
	m_vertices = src->m_vertices;
	m_indices = src->m_indices;
	m_origin = src->m_origin;
	m_width = src->m_width;
	m_height = src->m_height;
}
