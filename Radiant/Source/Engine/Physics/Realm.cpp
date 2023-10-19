#include "pch.h"
#include "Realm.h"
#include "Collision.h"
#include "Messaging/MessageTypes.h"

namespace rdt::core {

    Realm::Realm()
        : m_ID(GetUniqueID()), m_gravity(0)
    {
    }

    Realm::~Realm()
    {
        FreeUniqueID(m_ID);
    }

    void Realm::OnUpdate(const float deltaTime)
    {
        for (auto& [id1, object1] : m_objects) {

            object1.translation.UpdateVelocity(deltaTime);

            bool collisionDetected = false;

            if (!object1.HasProperties(NoCollision)) {

                for (auto& [id2, object2] : m_objects) {
                    if (id1 == id2) {
                        continue;
                    }

                    std::vector<UniqueID> sharedTags;
                    if (object1.GetSharedTags(object2, sharedTags)) {
                        bool skip = false;
                        for (auto tagID : sharedTags) {
                            if (PtagManager::GetTag(tagID).HasProperties(NoCollision)) {
                                skip = true;
                                break;
                            }
                        }
                        if (skip) {
                            continue;
                        }
                    }
                    

                    if (Collision::CheckCollision(object1, object2, deltaTime)) {
                        MessageBus::AddToQueue(m_object_mIDs[id2], m_object_mIDs[id1], MT_Collision, new CollisionData(id2));
                        collisionDetected = true;
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