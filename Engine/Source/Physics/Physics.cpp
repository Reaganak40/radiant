#include "pch.h"
#include "Physics.h"

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
    UniqueID Physics::CreateObjectImpl(const Polygon& nPolygon)
    {
        UniqueID nUUID = GetUniqueID();
        m_objects[nUUID] = Pobject(nPolygon);

        return nUUID;
    }
}