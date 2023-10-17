#include "pch.h"
#include "Translation.h"
#include "Utils/Utils.h"
#include "Polygon/PolygonImpl.h"

namespace rdt::core {
	
	Translation::Translation(Vec2d initial_velocity, Vec2d initial_acceleration)
		: m_current_velocity(initial_velocity), m_acceleration(initial_acceleration),
		m_friction(0.98), m_gravity{0.0, 0.0}
	{
		m_has_max_velocity = false;
		m_max_velocity = (0.0, 0.0);
	}

	Translation::~Translation()
	{
	}

	void Translation::SetMaxVelocity(const Vec2d& nMaxVelocity)
	{
		m_has_max_velocity = true;
		m_max_velocity = Vabs(nMaxVelocity);
	}

	void Translation::SetFriction(double frictionMagnitude) {
		m_friction = Utils::ApplyEpsilon(1 - fmin(frictionMagnitude, 1));
	}

	void Translation::SetVelocity(const Vec2d nVelocity) {
		m_current_velocity = nVelocity;
	}

	void Translation::SetGravity(double nGravity)
	{
		m_gravity.y = nGravity;
	}

	Vec2d Translation::GetVelocity() {
		return m_current_velocity;
	}

	void Translation::UpdateVelocity(const float deltaTime)
	{
		m_current_velocity += deltaTime * (m_acceleration + m_gravity);
		//m_current_velocity -= (m_current_velocity * deltaTime * m_friction);

		if (abs(m_current_velocity.x) < 1) {
			m_current_velocity.x = 0;
		}

		if (abs(m_current_velocity.y) < 1) {
			m_current_velocity.y = 0;
		}

		if (m_has_max_velocity) {
			if (abs(m_current_velocity.x) > m_max_velocity.x) {
				m_current_velocity.x = m_current_velocity.x < 0 ? -m_max_velocity.x : m_max_velocity.x;
				m_acceleration.x = 0;
			}

			if (abs(m_current_velocity.y) > m_max_velocity.y) {
				m_current_velocity.y = m_current_velocity.y < 0 ? -m_max_velocity.y : m_max_velocity.y;
				m_acceleration.y = 0;
			}
		}
	}

	void Translation::UpdateAcceleration(Vec2d dAcc)
	{
		m_acceleration += dAcc;
	}

	void Translation::Translate(std::vector<Vec2d>& vertices, const float deltaTime)
	{
		for (auto& vertex : vertices) {
			Translate(vertex, deltaTime);
		}
	}

	void Translation::Translate(Vec2d& point, const float deltaTime)
	{
			point += m_current_velocity * deltaTime;
			point.x = Utils::ApplyEpsilon(point.x);
			point.y = Utils::ApplyEpsilon(point.y);
	}

	void Translation::Translate(Polygon& polygon, const float deltaTime)
	{
		if (m_current_velocity == Vec2d::Zero()) {
			return;
		}
		Translate(polygon.m_impl->m_vertices, deltaTime);
		Translate(polygon.m_impl->m_origin, deltaTime);
	}

	Vec2d Translation::GetChangeInPosition(const float deltaTime)
	{
		return m_current_velocity * deltaTime;
	}

	void Translation::OnEndFrame()
	{
	}
}
