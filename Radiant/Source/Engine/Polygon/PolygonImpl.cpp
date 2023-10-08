#include "pch.h"
#include "PolygonImpl.h"
#include "Graphics/Renderer.h"

rdt::core::PolygonImpl::PolygonImpl()
	: m_UUID(GetUniqueID()), m_width(0), m_height(0), m_properties(0), m_rotation(0)
{
}

rdt::core::PolygonImpl::~PolygonImpl()
{
	FreeUniqueID(m_UUID);
	Renderer::_FlushPolygon(m_UUID);
}
