#include "pch.h"
#include "Realm.h"
#include "Collision.h"
#include "Messaging/MessageTypes.h"

#include "ECS/ECS.h"
#include "Utils/Utils.h"

#include "Collider.h"

namespace rdt::core {

    Realm::Realm()
    {
        is_active = false;
        m_gravity = Vec2d(0, -900);
    }

    Realm::~Realm()
    {
    }

    void Realm::PushEntity(Entity nEntity)
    {
        m_entities.push_back(nEntity);
    }

    void Realm::Flush()
    {
        m_entities.clear();
    }

    void Realm::SetActive(bool active)
    {
        is_active = active;
    }

    void Realm::OnUpdate(const float deltaTime)
    {
        if (!is_active) {
            return;
        }

        // Entities have already been verified to hold all these components
        auto rigidbodies = ComponentManager::GetComponent<RigidBody2D>();
        auto transforms = ComponentManager::GetComponent<Transform>();
        auto sprites = ComponentManager::GetComponent<Sprite>();

        // Create CollisionObjects made from each entity's collider and transform
        std::unordered_map<Entity, CollisionObject> entityCollisionData;
        auto getCollisionData = [&](Entity entity) -> CollisionObject& {
            if (entityCollisionData.find(entity) == entityCollisionData.end()) {
                entityCollisionData[entity];

                auto& collisionData = entityCollisionData.at(entity);
                auto& rigidbody = rigidbodies->GetData(entity);
                auto& transform = transforms->GetData(entity);
                auto& collider = ColliderManager::GetCollider(rigidbody.colliderID);

                collider.ApplyTransform(transform, collisionData.vertices);
                collisionData.isRect = collider.IsRect();
                collisionData.isAxisAligned = collider.IsAxisAligned() && (transform.rotation == 0.0f);
                collisionData.resolveCollision = true;

                collisionData.transform = &transform;
                collisionData.rigidBody = &rigidbody;
                collisionData.size = collider.GetSize(transform.scale);
                collisionData.midpoint = collider.GetMidpoint(transform.scale);
            }

            return entityCollisionData.at(entity);
            };

        // Update velocities
        for (auto entity : m_entities) {
            auto& rigidbody = rigidbodies->GetData(entity);

            Vec2d externalForce;
            if (rigidbody.use_gravity) {
                externalForce = m_gravity;
            }
            rigidbody.UpdateVelocity(deltaTime, externalForce);
        }

        // Cycle entities checking for collisions
        for (auto entity1 : m_entities) {

            auto& collisionObject1 = getCollisionData(entity1);

            for (auto entity2 : m_entities) {
                if (entity1 == entity2) {
                    continue;
                }

                auto& collisionObject2 = getCollisionData(entity2);
                if (Collision::CheckCollision(collisionObject1, collisionObject2, deltaTime)) {
                    // TODO: Notify/send collision data
                }
            }
        }

        // Do final translation
        for (auto entity : m_entities) {
            transforms->GetData(entity).Translate(deltaTime, rigidbodies->GetData(entity).velocity);
        }
    }

    void Realm::SetGravity(double mps2)
    {
        m_gravity.y = mps2;
    }
}