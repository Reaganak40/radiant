#include "pch.h"
#include "Circle.h"
#include "PolygonImpl.h"

namespace rdt {
	
	struct Circle::Impl {
		double m_radius;

		Impl(double radius)
			: m_radius(radius)
		{
		}

		~Impl()
		{
		}
	};

	Circle::Circle(Vec2d spawnLocation, double radius)
		: m_impl(new Circle::Impl(radius))
	{
		BaseImpl()->m_origin = spawnLocation;
		
		BaseImpl()->m_vertices.push_back(Vec2d(BaseImpl()->m_origin.x - radius, BaseImpl()->m_origin.y - radius));
		BaseImpl()->m_vertices.push_back(Vec2d(BaseImpl()->m_origin.x + radius, BaseImpl()->m_origin.y - radius));
		BaseImpl()->m_vertices.push_back(Vec2d(BaseImpl()->m_origin.x + radius, BaseImpl()->m_origin.y + radius));
		BaseImpl()->m_vertices.push_back(Vec2d(BaseImpl()->m_origin.x - radius, BaseImpl()->m_origin.y + radius));

		BaseImpl()->m_indices.push_back(0);
		BaseImpl()->m_indices.push_back(1);
		BaseImpl()->m_indices.push_back(2);
		BaseImpl()->m_indices.push_back(2);
		BaseImpl()->m_indices.push_back(3);
		BaseImpl()->m_indices.push_back(0);
	}

	Circle::~Circle()
	{
		delete m_impl;
	}

	double Circle::GetRadius() const
	{
		return m_impl->m_radius;
	}
}
