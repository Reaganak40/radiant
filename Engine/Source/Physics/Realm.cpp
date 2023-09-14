#include "pch.h"
#include "Realm.h"
#include "Collision.h"

namespace rdt {

    Realm::Realm()
        : m_ID(GetUniqueID())
    {
    }

    Realm::~Realm()
    {
        FreeUniqueID(m_ID);
    }

    void Realm::OnUpdate(const float deltaTime)
    {
        for (auto& [id, object] : m_objects) {
            if (!object.HasProperties(NoCollision)) {
                object.ResetCollisions();
            }
        }

        for (auto& [id1, object1] : m_objects) {

            object1.translation.UpdateVelocity(deltaTime);

            bool collisionDetected = false;

            if (!object1.HasProperties(NoCollision)) {

                for (auto& [id2, object2] : m_objects) {
                    if (id1 == id2 || !object1.ShareTags(object2)) {
                        continue;
                    }

                    if (Collision::CheckCollision(object1, object2, deltaTime)) {
                        collisionDetected = true;
                        object1.AddCollision(id2);
                        object2.AddCollision(id1);
                    }
                }
            }


            if (object1.HasProperties(NoCollision) || !collisionDetected || !object1.HasProperties(ppBouncy)) {
                object1.translation.Translate(*object1.m_polygon, deltaTime);
            }
        }
	}
    void Realm::OnEndFrame()
    {
        for (auto& [id, object] : m_objects) {
            object.translation.OnEndFrame();
        }
    }
    const UniqueID Realm::CreatePhysicsObject(std::shared_ptr<Polygon> polygon)
    {
        UniqueID objectID = GetUniqueID();
        m_objects[objectID] = Pobject(polygon);
        return objectID;
    }

    Pobject* Realm::GetPhysicsObject(const UniqueID UUID)
    {
        if (m_objects.find(UUID) == m_objects.end()) {
            return nullptr;
        }

        return &m_objects.at(UUID);
    }
}