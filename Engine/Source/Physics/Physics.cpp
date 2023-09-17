#include "pch.h"
#include "Physics.h"
#include "Ptag.h"
#include "Collision.h"

#include "Utils/Input.h"
#include "Renderer/Renderer.h"

namespace rdt {

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
        for (auto& realmID : m_active_realms) {
            m_realms.at(realmID)->OnUpdate(deltaTime);
        }
    }

    void Physics::OnEndFrameImpl()
    {
        for (auto& realmID : m_active_realms) {
            m_realms.at(realmID)->OnEndFrame();
        }
    }

    UniqueID Physics::CreateRealmImpl()
    {
        Realm* nRealm = new Realm;
        m_realms[nRealm->GetUUID()] = nRealm;

        return nRealm->GetUUID();
    }

    void Physics::ActivateRealmImpl(const UniqueID realmID)
    {
        m_active_realms.insert(realmID);
    }

    void Physics::DeactivateRealmImpl(const UniqueID realmID)
    {
        m_active_realms.erase(realmID);
    }

    const Polygon& Physics::GetPolygonImpl(const UniqueID realmID, const UniqueID objectID)
    {
        if (m_realms.find(realmID) == m_realms.end()) {
            return m_bad_poly;
        }

        Pobject* object = m_realms.at(realmID)->GetPhysicsObject(objectID);
        if (object == nullptr) {
            return m_bad_poly;
        }

        return *object->m_polygon;
    }

    void Physics::SetObjectPropertiesImpl(const UniqueID realmID, const UniqueID objectID, const unsigned int nProperties)
    {
        if (m_realms.find(realmID) == m_realms.end()) {
            return;
        }

        Pobject* object = m_realms.at(realmID)->GetPhysicsObject(objectID);
        if (object == nullptr) {
            return;
        }

        object->SetProperties(nProperties);
    }

    void Physics::RemoveObjectPropertiesImpl(const UniqueID realmID, const UniqueID objectID, const unsigned int rProperties)
    {
        if (m_realms.find(realmID) == m_realms.end()) {
            return;
        }

        Pobject* object = m_realms.at(realmID)->GetPhysicsObject(objectID);
        if (object == nullptr) {
            return;
        }

        object->RemoveProperties(rProperties);
    }

    bool Physics::QueryObjectPropertiesImpl(const UniqueID realmID, const UniqueID objectID, const unsigned int propertyQuery)
    {
        if (m_realms.find(realmID) == m_realms.end()) {
            return false;
        }

        Pobject* object = m_realms.at(realmID)->GetPhysicsObject(objectID);
        if (object == nullptr) {
            return false;
        }

        return object->HasProperties(propertyQuery);
    }

    UniqueID Physics::CreateObjectImpl(const UniqueID realmID, const MessageID messageID, std::shared_ptr<Polygon> polygon)
    {
        if (m_realms.find(realmID) == m_realms.end()) {
            return 0;
        }
        return m_realms.at(realmID)->CreatePhysicsObject(polygon, messageID);
    }

    void Physics::AddPTagImpl(const std::string& tagName, const UniqueID realmID, const UniqueID objectID)
    {
        if (m_realms.find(realmID) == m_realms.end()) {
            return;
        }

        Pobject* object = m_realms.at(realmID)->GetPhysicsObject(objectID);
        if (object == nullptr) {
            return;
        }

        Ptag ntag;
        if ((ntag = PtagManager::GetTag(tagName)) == 0) {
            ntag = PtagManager::CreateTag(tagName);
        }

        object->AddTag(ntag);
    }

    void Physics::SetAccelerationImpl(const UniqueID realmID, const UniqueID objectID, const Vec2d& nAcceleration)
    {
        if (m_realms.find(realmID) == m_realms.end()) {
            return;
        }

        Pobject* object = m_realms.at(realmID)->GetPhysicsObject(objectID);
        if (object == nullptr) {
            return;
        }

        object->translation.m_acceleration = nAcceleration;
    }

    void Physics::SetAccelerationXImpl(const UniqueID realmID, const UniqueID objectID, const double nX)
    {
        if (m_realms.find(realmID) == m_realms.end()) {
            return;
        }

        Pobject* object = m_realms.at(realmID)->GetPhysicsObject(objectID);
        if (object == nullptr) {
            return;
        }

        object->translation.m_acceleration.x = nX;
    }

    void Physics::SetAccelerationYImpl(const UniqueID realmID, const UniqueID objectID, const double nY)
    {
        if (m_realms.find(realmID) == m_realms.end()) {
            return;
        }

        Pobject* object = m_realms.at(realmID)->GetPhysicsObject(objectID);
        if (object == nullptr) {
            return;
        }

        object->translation.m_acceleration.y = nY;
    }

    void Physics::SetVelocityImpl(const UniqueID realmID, const UniqueID objectID, Vec2d& nVelocity)
    {
        if (m_realms.find(realmID) == m_realms.end()) {
            return;
        }

        Pobject* object = m_realms.at(realmID)->GetPhysicsObject(objectID);
        if (object == nullptr) {
            return;
        }

        object->translation.SetVelocity(nVelocity);
    }

    void Physics::SetMaximumVelocityImpl(const UniqueID realmID, const UniqueID objectID, const Vec2d& nMaxVelocity)
    {
        if (m_realms.find(realmID) == m_realms.end()) {
            return;
        }

        Pobject* object = m_realms.at(realmID)->GetPhysicsObject(objectID);
        if (object == nullptr) {
            return;
        }

        object->translation.SetMaxVelocity(nMaxVelocity);
    }

    void Physics::SetFrictionImpl(const UniqueID realmID, const UniqueID objectID, const double friction)
    {
        if (m_realms.find(realmID) == m_realms.end()) {
            return;
        }

        Pobject* object = m_realms.at(realmID)->GetPhysicsObject(objectID);
        if (object == nullptr) {
            return;
        }

        object->translation.SetFriction(friction);
    }

    void Physics::SetPositionImpl(const UniqueID realmID, const UniqueID objectID, const Vec2d& nPosition)
    {
        if (m_realms.find(realmID) == m_realms.end()) {
            return;
        }

        Pobject* object = m_realms.at(realmID)->GetPhysicsObject(objectID);
        if (object == nullptr) {
            return;
        }

        object->m_polygon->SetPosition(nPosition);
    }

    void Physics::SetRotationImpl(const UniqueID realmID, const UniqueID objectID, const double nRadians)
    {
        if (m_realms.find(realmID) == m_realms.end()) {
            return;
        }

        Pobject* object = m_realms.at(realmID)->GetPhysicsObject(objectID);
        if (object == nullptr) {
            return;
        }

        object->m_polygon->SetRotation(nRadians);
    }

    void Physics::SetHitBoxSizeImpl(const UniqueID realmID, const UniqueID objectID, const Vec2d& nSize)
    {
        if (m_realms.find(realmID) == m_realms.end()) {
            return;
        }

        Pobject* object = m_realms.at(realmID)->GetPhysicsObject(objectID);
        if (object == nullptr) {
            return;
        }

        object->SetHitBoxSize(nSize);
    }

    Vec2d Physics::GetVelocityImpl(const UniqueID realmID, const UniqueID objectID)
    {
        if (m_realms.find(realmID) == m_realms.end()) {
            return Vec2d::Zero();
        }

        Pobject* object = m_realms.at(realmID)->GetPhysicsObject(objectID);
        if (object == nullptr) {
            return Vec2d::Zero();
        }

        return object->translation.GetVelocity();
    }
}