#include "pch.h"
#include "CommonComponents.h"
#include "EntityManager.h"

namespace rdt {

	void Renderable::AddPolygonFromRigidBody(Entity entity)
	{
		auto rigidBody = EntityManager::GetComponent<RigidBody2D>(entity);
		auto renderable = EntityManager::GetComponent<Renderable>(entity);

		if (rigidBody == nullptr || renderable == nullptr) {
			return;
		}

		renderable->polygon = rigidBody->polygon;
	}
}
