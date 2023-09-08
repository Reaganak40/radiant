#include "pch.h"
#include "Physics.h"
#include "Collision.h"

#include "Utils/Input.h"
#include "Renderer/Renderer.h"

namespace Radiant {

    Physics* Physics::m_instance = nullptr;

    Physics::Physics() {

    }

    Physics::~Physics()
    {
    }

    Physics* Physics::GetInstance()
    {
        if (m_instance == nullptr) {
            m_instance = new Physics;
        }

        return m_instance;
    }
    void Physics::Initialize()
    {
        Destroy();
        GetInstance();
    }

    void Physics::Destroy()
    {
        if (m_instance != nullptr) {
            delete m_instance;
            m_instance = nullptr;
        }
    }
    void Physics::OnUpdateImpl(const float deltaTime)
    {
        for (auto& [id1, object1] : m_objects) {

            object1.translation.UpdateVelocity(deltaTime);

            bool collisionDetected = false;
            
            if (!object1.HasProperties(ppRigid)) {

                for (auto& [id2, object2] : m_objects) {
                    if (id1 == id2) {
                        continue;
                    }

                    if (Collision::SweptAABB(object1, object2, deltaTime)) {
                        collisionDetected = true;
                    }
                }
            }

            if (!collisionDetected || !object1.HasProperties(ppBouncy)) {
                object1.translation.Translate(*object1.m_polygon, deltaTime);
            }
        }
    }

    void Physics::OnEndFrameImpl()
    {
        for (auto& [id, object] : m_objects) {
            object.translation.OnEndFrame();
        }
    }

    void Physics::SetObjectPropertiesImpl(const UniqueID objectID, const unsigned int nProperties)
    {
        if (m_objects.find(objectID) == m_objects.end()) {
            return;
        }

        m_objects.at(objectID).SetProperties(nProperties);
    }

    void Physics::RemoveObjectPropertiesImpl(const UniqueID objectID, const unsigned int rProperties)
    {
        if (m_objects.find(objectID) == m_objects.end()) {
            return;
        }

        m_objects.at(objectID).RemoveProperties(rProperties);
    }

    bool Physics::QueryObjectPropertiesImpl(const UniqueID objectID, const unsigned int propertyQuery)
    {
        if (m_objects.find(objectID) == m_objects.end()) {
            return false;
        }

        return m_objects.at(objectID).HasProperties(propertyQuery);
    }

    UniqueID Physics::CreateObjectImpl(std::shared_ptr<Polygon> polygon)
    {
        UniqueID nUUID = GetUniqueID();
        m_objects[nUUID] = Pobject(polygon);

        return nUUID;
    }
    void Physics::SetAccelerationImpl(UniqueID UUID, const Vec2d& nAcceleration)
    {
        if (m_objects.find(UUID) == m_objects.end()) {
            return;
        }

        m_objects.at(UUID).translation.m_acceleration = nAcceleration;
    }

    void Physics::SetAccelerationXImpl(UniqueID UUID, const double nX)
    {
        if (m_objects.find(UUID) == m_objects.end()) {
            return;
        }

        m_objects.at(UUID).translation.m_acceleration.x = nX;
    }

    void Physics::SetAccelerationYImpl(UniqueID UUID, const double nY)
    {
        if (m_objects.find(UUID) == m_objects.end()) {
            return;
        }

        m_objects.at(UUID).translation.m_acceleration.y = nY;
    }

    void Physics::SetVelocityImpl(UniqueID objectID, Vec2d& nVelocity)
    {
        if (m_objects.find(objectID) == m_objects.end()) {
            return;
        }

        m_objects.at(objectID).translation.SetVelocity(nVelocity);
    }

    void Physics::SetMaximumVelocityImpl(UniqueID UUID, const Vec2d& nMaxVelocity)
    {
        if (m_objects.find(UUID) == m_objects.end()) {
            return;
        }

        m_objects.at(UUID).translation.SetMaxVelocity(nMaxVelocity);
    }

    void Physics::SetFrictionImpl(const UniqueID objectID, const double friction)
    {
        if (m_objects.find(objectID) == m_objects.end()) {
            return;
        }

        m_objects.at(objectID).translation.SetFriction(friction);
    }

    void Physics::SetPositionImpl(const UniqueID objectID, const Vec2d& nPosition) {
        if (m_objects.find(objectID) == m_objects.end()) {
            return;
        }

        m_objects.at(objectID).m_polygon->SetPosition(nPosition);
    }

}