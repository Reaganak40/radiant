#include "pch.h"
#include "PhysicsSystem.h"

namespace rdt {
	PhysicsSystem::PhysicsSystem()
	{
		Signature nSignature;
		ComponentManager::UpdateSignature<RigidBody2D>(nSignature);
		SetSignature(nSignature);
	}

	PhysicsSystem::~PhysicsSystem()
	{
	}

	void PhysicsSystem::Update(float deltaTime)
	{
		auto& entities = System::GetEntities();
		auto rigidBodies = System::GetComponent<RigidBody2D>();
	}
}
