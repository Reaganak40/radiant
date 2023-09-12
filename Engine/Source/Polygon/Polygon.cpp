#include "pch.h"
#include "Polygon.h"
#include "Renderer/Renderer.h"
#include "Utils/Utils.h"

namespace rdt {
    Polygon::Polygon()
        : m_UUID(GetUniqueID()), m_width(0), m_height(0), m_properties(0), m_rotation(0)
    {
    }
    Polygon::~Polygon()
    {
        FreeUniqueID(m_UUID);
        Renderer::_FlushPolygon(m_UUID);
    }

    void Polygon::AddProperties(unsigned int nProperties)
    {
        m_properties |= nProperties;
    }

    const std::vector<Vec2d>& Polygon::GetVertices() const
    {
        return m_vertices;
    }

    bool Polygon::CheckProperties(unsigned int propertyQuery)
    {
        return (bool)(m_properties & propertyQuery);
    }

    const std::vector<unsigned int> Polygon::GetIndices() const
    {
        return m_indices;
    }

    Vec2d Polygon::GetOrigin() const
    {
        return m_origin;
    }

    double Polygon::GetWidth() const
    {
        return m_width;
    }

    double Polygon::GetHeight() const
    {
        return m_height;
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

    void Polygon::SetPosition(const Vec2d& nPosition)
    {
        std::vector<Vec2d> offsets;
        offsets.reserve(m_vertices.size());

        for (const auto& vertex : m_vertices) {
            offsets.push_back(Utils::GetManhattanDistance(m_origin, vertex));
        }

        m_origin = nPosition;

        for (unsigned int i = 0; i < m_vertices.size(); i++) {
            m_vertices[i] = m_origin + offsets[i];
        }
    }

    void Polygon::ApplyRotationOffset(const double oRadians)
    {
        SetRotation(oRadians);
        m_rotation = 0;
    }

    void Polygon::SetRotation(const double nRadians)
    {
        double dr = nRadians - m_rotation;

        for (auto& vertex : m_vertices) {
            Utils::RotatePoint(m_origin, vertex, dr);
        }

        m_rotation = nRadians;
    }
}