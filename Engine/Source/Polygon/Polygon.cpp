#include "pch.h"
#include "Polygon.h"

namespace Radiant {

    const std::vector<Vec2d>& Polygon::GetVertices() const
    {
        return m_vertices;
    }

    const std::vector<unsigned int> Polygon::GetIndices() const
    {
        return m_indices;
    }

    Vec2d Polygon::GetOrigin() const
    {
        return m_origin;
    }

    const UniqueID Polygon::GetUUID() const
    {
        return m_UUID;
    }

    void Polygon::Move(double dx, double dy)
    {
        for (auto& vertex : m_vertices) {
            vertex.x += dx;
            vertex.y += dy;
        }

        m_origin.x += dx;
        m_origin.y += dy;
    }
}