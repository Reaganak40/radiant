#include "pch.h"
#include "Physics.h"
#include "Collision.h"

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
            bool flag = false;
            for (auto& [id2, object2] : m_objects) {
                if (id1 == id2) {
                    continue;
                }

                flag = Collision::SweptAABB(object1, object2, deltaTime);
            }

            object1.translation.Translate(*object1.m_polygon, deltaTime);
        }
    }

    void Physics::OnEndFrameImpl()
    {
        for (auto& [id, object] : m_objects) {
            object.translation.OnEndFrame();
        }
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

    void Physics::SetMaximumVelocityImpl(UniqueID UUID, const Vec2d& nMaxVelocity)
    {
        if (m_objects.find(UUID) == m_objects.end()) {
            return;
        }

        m_objects.at(UUID).translation.SetMaxVelocity(nMaxVelocity);
    }
}