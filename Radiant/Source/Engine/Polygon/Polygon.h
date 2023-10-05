#pragma once
#include "Core.h"
#include "Utils/MathTypes.h"
#include "Utils/UniqueID.h"

namespace rdt::core {
	class Translation;
}

namespace rdt {

	enum PolygonProperies {
		IsRect = 1,
		IsLine = 2,
	};

	class RADIANT_API Polygon {
	private:
		UniqueID m_UUID;
		double m_rotation;
	protected:
		unsigned int m_properties;
		std::vector<Vec2d> m_vertices;
		std::vector<unsigned int> m_indices;

		Vec2d m_origin;
		double m_width;
		double m_height;

		void AddProperties(unsigned int nProperties);
	public:
		Polygon();
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
			Gets the rotation of this polygon in radians.
		*/
		double GetRotation() const;

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


		/*
			Sets the initial rotation of the object to a different degree in radians
			other than 0. This way, the axes may be aligned in arbitrary cases.

			This should be done before SetRotation() is first used.
		*/
		void ApplyRotationOffset(const double oRadians);

		/*
			Rotates the object to the specified angle in radians.
		*/
		void SetRotation(const double nRadians);

		friend class core::Translation;
		friend class Renderer;
	};
}