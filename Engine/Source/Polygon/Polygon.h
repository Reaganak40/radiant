#pragma once
#include "pch.h"
#include "Utils/MathTypes.h"
#include "Utils/UniqueID.h"

namespace rdt {

	enum PolygonProperies {
		IsRect = 1,
		IsLine = 2,
	};

	class Polygon {
	private:
		UniqueID m_UUID;
	protected:
		unsigned int m_properties;
		std::vector<Vec2d> m_vertices;
		std::vector<unsigned int> m_indices;

		Vec2d m_origin;
		double m_width;
		double m_height;

		void AddProperties(unsigned int nProperties);
	public:
		Polygon() : m_UUID(GetUniqueID()), m_width(0), m_height(0), m_properties(0) {}
		~Polygon();

		/*
			Get the array of vertices for this polygon.
		*/
		const std::vector<Vec2d>& GetVertices() const;

		/*
			Query the properties of this polygon, returning true if any are true.
		*/
		bool CheckProperties(unsigned int propertyQuery);

		/*
			Get the array of indicies that represent the triangles to create this polygon.
		*/
		const std::vector<unsigned int> GetIndices() const;

		/*
			Get the origin coordiantes for this polygon.
		*/
		Vec2d GetOrigin() const;

		/*
			Get the width of this polygon.
		*/
		double GetWidth() const;

		/*
			Get the height of this polygon.
		*/
		double GetHeight() const;

		/*
			Returns the unique identifier for this polygon.
		*/
		const UniqueID GetUUID() const;

		/*
			Moves the polygon, its origin and vertices by the specified dx and dy.
		*/
		void Move(double dx, double dy);

		/*
			Sets the position of the polygon, moving its origin and vertices.
		*/
		virtual void SetPosition(const Vec2d& nPosition);

		/*
			Sets the size of the polygon.
		*/
		virtual void SetSize(const Vec2d& nSize) { printf("Warning: Using abstract class SetSize()!"); }

		friend class Translation;
		friend class Renderer;
	};
}