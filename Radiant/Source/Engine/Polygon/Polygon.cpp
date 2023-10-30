#include "pch.h"
#include "Polygon.h"
#include "PolygonImpl.h"

#include "Utils/Utils.h"

namespace rdt {
    Polygon::Polygon()
        : m_impl(new core::PolygonImpl)
    {
    }

    Polygon::~Polygon()
    {
        delete m_impl;
    }

    Polygon::Polygon(const Polygon& polygon)
        : m_impl(new core::PolygonImpl)
    {
        m_impl->Copy(polygon.m_impl);
    }

    void Polygon::Copy(const Polygon& polygon)
    {
        m_impl->Copy(polygon.m_impl);
    }


    core::PolygonImpl* Polygon::BaseImpl()
    {
        return m_impl;
    }

    void Polygon::AddProperties(unsigned int nProperties)
    {
        m_impl->m_properties |= nProperties;
    }

    const std::vector<Vec2d>& Polygon::GetVertices() const
    {
        return m_impl->m_vertices;
    }

    bool Polygon::CheckProperties(unsigned int propertyQuery)
    {
        return (bool)(m_impl->m_properties & propertyQuery);
    }

    const std::vector<unsigned int> Polygon::GetIndices() const
    {
        return m_impl->m_indices;
    }

    Vec2d Polygon::GetOrigin() const
    {
        return m_impl->m_origin;
    }

    double Polygon::GetWidth() const
    {
        return m_impl->m_width;
    }

    double Polygon::GetHeight() const
    {
        return m_impl->m_height;
    }

    double Polygon::GetRotation() const
    {
        return m_impl->m_rotation;
    }

    const UniqueID Polygon::GetUUID() const
    {
        return m_impl->m_UUID;
    }

    void Polygon::Move(double dx, double dy)
    {
        for (auto& vertex : m_impl->m_vertices) {
            vertex.x += dx;
            vertex.y += dy;
        }

        m_impl->m_origin.x += dx;
        m_impl->m_origin.y += dy;
    }

    void Polygon::SetPosition(const Vec2d& nPosition)
    {
        std::vector<Vec2d> offsets;
        offsets.reserve(m_impl->m_vertices.size());

        for (const auto& vertex : m_impl->m_vertices) {
            offsets.push_back(Utils::GetManhattanDistance(m_impl->m_origin, vertex));
        }

        m_impl->m_origin = nPosition;

        for (unsigned int i = 0; i < m_impl->m_vertices.size(); i++) {
            m_impl->m_vertices[i] = m_impl->m_origin + offsets[i];
        }
    }

    void Polygon::ApplyRotationOffset(const double oRadians)
    {
        SetRotation(oRadians);
        m_impl->m_rotation = 0;
    }

    void Polygon::SetRotation(const double nRadians)
    {
        double dr = nRadians - m_impl->m_rotation;

        for (auto& vertex : m_impl->m_vertices) {
            Utils::RotatePoint(m_impl->m_origin, vertex, dr);
        }

        m_impl->m_rotation = nRadians;
    }

    void Polygon::_DontFlush()
    {
        m_impl->shouldFlush = false;
    }

}