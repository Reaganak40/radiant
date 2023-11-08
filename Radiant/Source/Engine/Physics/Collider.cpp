#include "pch.h"
#include "Collider.h"

#include "ECS/ECS.h"
#include "Utils/Utils.h"
#include "Logging/Log.h"

rdt::Collider::Collider()
{
	m_is_rect = false;
	m_is_axis_aligned = false;
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

bool rdt::Collider::IsRect()
{
	return m_is_rect;
}

bool rdt::Collider::IsAxisAligned()
{
	return m_is_axis_aligned;
}

rdt::Vec2d rdt::Collider::GetSize(Vec2d scale)
{
	return m_size * scale;
}

rdt::Vec2d rdt::Collider::GetMidpoint(Vec2d scale)
{
	return m_midpoint * scale;
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
	m_size = size;
	
	m_midpoint.x = origin.x + (size.x / 2);
	m_midpoint.y = origin.y + (size.y / 2);

	m_is_rect = true;
	m_is_axis_aligned = true;
}

// ============================================================================
struct rdt::ColliderManager::Impl {
	ColliderID idCounter = 0;

	std::unordered_map<std::string, ColliderID> aliasToID;
	std::unordered_map<ColliderID, Collider> m_profiles;

	ColliderID GenerateID() {
		return ++idCounter;
	}
	bool ColliderExists(ColliderID cpID) {
		return m_profiles.find(cpID) != m_profiles.end();
	}

	bool ColliderExists(const std::string& alias) {
		return aliasToID.find(alias) != aliasToID.end();
	}

	ColliderID GetColliderID(const std::string& alias) {
		return aliasToID.at(alias);
	}

	ColliderID RegisterCollider(const std::string& alias) {
		if (ColliderExists(alias)) {
			RDT_WARN("ColliderManager - Tried to register duplicate collider '{}'", alias);
			return GetColliderID(alias);
		}

		ColliderID nID = GenerateID();
		aliasToID[alias] = nID;
		m_profiles[nID];
		return nID;
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

rdt::ColliderID rdt::ColliderManager::GetColliderID(const std::string& alias)
{
	if (!m_impl->ColliderExists(alias)) {
		return RDT_NULL_COLLIDER_ID;
	}

	return m_impl->GetColliderID(alias);
}

bool rdt::ColliderManager::ColliderExists(ColliderID cpID)
{
	return m_impl->ColliderExists(cpID);
}

rdt::ColliderID rdt::ColliderManager::RegisterColider(const std::string& name)
{
	return m_impl->RegisterCollider(name);
}

rdt::Collider& rdt::ColliderManager::GetCollider(ColliderID cpID)
{
	return m_impl->m_profiles.at(cpID);
}
