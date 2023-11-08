#include "pch.h"
#include "Collider.h"

#include "ECS/ECS.h"
#include "Utils/Utils.h"
#include "Logging/Log.h"

rdt::Collider::Collider()
{
}

rdt::Collider::~Collider()
{
}

void rdt::Collider::ApplyTransform(const Transform& transform, std::vector<Vec2d>& vertices)
{
	for (const auto& vertex : m_vertices) {
		vertices.push_back(Utils::Scale(Vec2f::Zero(), vertex, transform.scale));
		Utils::RotatePoint(Vec2d::Zero(), vertices.back(), transform.rotation);
		Utils::Translate(vertices.back(), transform.position);
	}
}

void rdt::Collider::WarningUndefinedShape()
{
	RDT_CORE_ERROR("ColliderProfile - Missing shape definition");
}

void rdt::Collider::AddRect(const Vec2f& origin, const Vec2f& size)
{
	m_vertices.push_back(Vec2f(origin.x, origin.y));
	m_vertices.push_back(Vec2f(origin.x + size.x, origin.y));
	m_vertices.push_back(Vec2f(origin.x + size.x, origin.y + size.y));
	m_vertices.push_back(Vec2f(origin.x, origin.y + size.y));
}

// ============================================================================
struct rdt::ColliderManager::Impl {
	ColliderID idCounter = 0;

	std::unordered_map<ColliderID, Collider> m_profiles;

	bool ColliderExists(ColliderID cpID) {
		return m_profiles.find(cpID) != m_profiles.end();
	}

	ColliderID AddColliderProfile(const Collider& profile) {
		ColliderID nID = ++idCounter;

		m_profiles[nID] = profile;
		return nID;
	}
};
// ============================================================================
rdt::ColliderManager::Impl* rdt::ColliderManager::m_impl = nullptr;

void rdt::ColliderManager::Initialize()
{
	Destroy();
	m_impl = new ColliderManager::Impl;
}

void rdt::ColliderManager::Destroy()
{
	if (m_impl != nullptr) {
		delete m_impl;
		m_impl = nullptr;
	}
}

bool rdt::ColliderManager::ColliderExists(ColliderID cpID)
{
	return m_impl->ColliderExists(cpID);
}

rdt::ColliderID rdt::ColliderManager::AddCollider(const Collider& profile)
{
	return m_impl->AddColliderProfile(profile);
}

rdt::Collider& rdt::ColliderManager::GetCollider(ColliderID cpID)
{
	return m_impl->m_profiles.at(cpID);
}
