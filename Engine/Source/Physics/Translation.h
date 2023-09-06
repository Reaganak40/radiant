#pragma once
#include "Utils/MathTypes.h"
#include "Polygon/Polygon.h"

namespace Radiant {

	class Translation {
	private:

		Vec2d m_old_velocity;
		Vec2d m_current_velocity;

		bool m_has_max_velocity;
		Vec2d m_max_velocity;

		Vec2d m_acceleration;

		double m_friction;
		bool use_old_velocity;
	public:

		Translation(Vec2d initial_velocity = Vec2d(), Vec2d initial_acceleration = Vec2d());
		~Translation();


		/*
			Sets a maximum velocity for the velocity vector.
		*/
		void SetMaxVelocity(const Vec2d& nMaxVelocity);

		/*
			Sets the friction coefficient (0 = no friction, 1 = maximum friction) 
		*/
		void SetFriction(double frictionMagnitude);

		/*
			Sets the current velocity to the provided vector.
		*/
		void SetVelocity(const Vec2d nVelocity);

		/*
			Gets the current velocity vector.
		*/
		Vec2d GetVelocity();
		
		/*
			Updates the velocity vector according to dt and acceleration.
		*/
		void UpdateVelocity(const float deltaTime);

		/*
			Sets velocity to garuauntee that it will be this value in translation, and
			not the average between this and the old velocity.
		*/
		void SetRealVelocity(Vec2d nVelocity);

		/*
			Updates the acceleration vector by adding dAcc to it.
		*/
		void UpdateAcceleration(Vec2d dAcc);

		/*
			Updates all the vertices provided according to the translation vector.
		*/
		void Translate(std::vector<Vec2d>& vertices, const float deltaTime);

		/*
			Updates the point provided according to the translation vector
		*/
		void Translate(Vec2d& point, const float deltaTime);

		/*
			Updates the polygon provided according to the translation vector
		*/
		void Translate(Polygon& polygon, const float deltaTime);

		/*
			Gets this time frame's change in position according to the velocity vector.
		*/
		Vec2d GetChangeInPosition(const float deltaTime);

		/*
			Returns the projected new position of this point according to the velocity vector
			and the provided deltaTime.
		*/
		Vec2d GetNextPositionPoint(const Vec2d& point, const float deltaTime);

		/*
			Returns the projected new position of this list of vertices according to the velocity vector
			and the provided deltaTime.
		*/
		std::vector<Vec2d> GetNextPositionVertices(const std::vector<Vec2d>& vertices, const float deltaTime);

		/*
			Saves a snapshot of final calculations this frame for future dt calculations.
		*/
		void OnEndFrame();


		friend class Physics;
		friend class Collision;
	};

}