#include "pch.h"
#include "Realm.h"
#include "Collision.h"
#include "Messaging/MessageTypes.h"

#include "ECS/ECS.h"
#include "Utils/Utils.h"

#include "Collider.h"

namespace rdt::core {

    Realm::Realm()
        : m_ID(GetUniqueID()), m_gravity(0)
    {
    }

    Realm::~Realm()
    {
        FreeUniqueID(m_ID);
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
                auto& rigidbody = rigidbodies->GetData(entity);
                auto& transform = transforms->GetData(entity);
                ColliderManager::GetCollider(rigidbody.colliderID).ApplyTransform(transform, entityCollisionData.at(entity).vertices);
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

                }
            }
        }

        // Do final translation
        for (auto entity : m_entities) {
            transforms->GetData(entity).Translate(deltaTime, rigidbodies->GetData(entity).velocity);
        }
	}

    void Realm::OnEndFrame()
    {
        for (auto& [id, object] : m_objects) {
            object.translation.OnEndFrame();
        }
    }
    const UniqueID Realm::CreatePhysicsObject(std::shared_ptr<Polygon> polygon, const MessageID messageID)
    {
        UniqueID objectID = GetUniqueID();
        m_objects[objectID] = Pobject(polygon);
        m_object_mIDs[objectID] = messageID;
        
        m_objects.at(objectID).SetGravity(m_gravity);
        return objectID;
    }

    std::shared_ptr<Polygon> Realm::DestroyPhysicsObject(const UniqueID UUID)
    {
        if (m_objects.find(UUID) == m_objects.end()) {
            return std::shared_ptr<Polygon>();
        }

        std::shared_ptr<Polygon> polygon = m_objects.at(UUID).m_polygon;
        m_objects.erase(UUID);
        return polygon;
    }

    Pobject* Realm::GetPhysicsObject(const UniqueID UUID)
    {
        if (m_objects.find(UUID) == m_objects.end()) {
            return nullptr;
        }

        return &m_objects.at(UUID);
    }

    void Realm::SetGravity(double mps2)
    {
        m_gravity = mps2;
        for (auto& [id, object] : m_objects) {
            object.SetGravity(m_gravity);
        }
    }
}