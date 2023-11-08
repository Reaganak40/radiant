#include "pch.h"
#include "Physics.h"
#include "Ptag.h"
#include "Collision.h"
#include "Utils/Input.h"

namespace rdt {
    using namespace core;

    struct Physics::Impl {

        RealmID idCounter = 0;
        std::unordered_map<RealmID, core::Realm> m_realms;

        Impl()
        {

        }

        ~Impl()
        {

        }

        RealmID GenerateID() {
            return ++idCounter;
        }

        RealmID CreateRealm() {
            RealmID nID = GenerateID();
            m_realms[nID];
            return nID;
        }

        bool RealmExists(RealmID realm) {
            return m_realms.find(realm) != m_realms.end();
        }

        void SetRealmActive(const UniqueID realmID, bool active)
        {
            if (!RealmExists(realmID)) {
                RDT_WARN("Physics:: Could not find realm [id:{}] to activate.", realmID);
                return;
            }

            m_realms.at(realmID).SetActive(active);
        }

        bool AddEntityToRealm(const RealmID realm, const Entity entity)
        {
            if (!RealmExists(realm)) {
                return false;
            }

            m_realms.at(realm).PushEntity(entity);
            return true;
        }

        void UpdateRealms(const float deltaTime)
        {
            for (auto& [id, realm] : m_realms) {
                realm.OnUpdate(deltaTime);
            }
        }

        const Polygon& GetPolygon(const UniqueID realmID, const UniqueID objectID)
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

        void SetObjectProperties(const UniqueID realmID, const UniqueID objectID, PhysicalProperties nProperties)
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

        void RemoveObjectProperties(const UniqueID realmID, const UniqueID objectID, PhysicalProperties rProperties)
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

        bool QueryObjectProperties(const UniqueID realmID, const UniqueID objectID, PhysicalProperties propertyQuery)
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

        UniqueID CreateObject(const UniqueID realmID, const MessageID messageID, std::shared_ptr<Polygon> polygon)
        {
            if (m_realms.find(realmID) == m_realms.end()) {
                return 0;
            }
            return m_realms.at(realmID)->CreatePhysicsObject(polygon, messageID);
        }

        void AddPTag(const std::string& tagName, const UniqueID realmID, const UniqueID objectID)
        {
            if (m_realms.find(realmID) == m_realms.end()) {
                return;
            }

            Pobject* object = m_realms.at(realmID)->GetPhysicsObject(objectID);
            if (object == nullptr) {
                return;
            }

            UniqueID nPtag;
            if ((nPtag = PtagManager::GetTagID(tagName)) == 0) {
                nPtag = PtagManager::CreateTag(tagName);
            }

            object->AddTag(nPtag);
        }

        void SetAcceleration(const UniqueID realmID, const UniqueID objectID, const Vec2d& nAcceleration)
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

        void SetAccelerationX(const UniqueID realmID, const UniqueID objectID, const double nX)
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

        void SetAccelerationY(const UniqueID realmID, const UniqueID objectID, const double nY)
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

        void SetVelocity(const UniqueID realmID, const UniqueID objectID, Vec2d& nVelocity)
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

        void SetMaximumVelocity(const UniqueID realmID, const UniqueID objectID, const Vec2d& nMaxVelocity)
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

        void SetGravity(const UniqueID realmID, double mps2)
        {
            if (m_realms.find(realmID) == m_realms.end()) {
                return;
            }

            m_realms.at(realmID)->SetGravity(mps2);
        }

        void SetFriction(const UniqueID realmID, const UniqueID objectID, const double friction)
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

        void SetPosition(const UniqueID realmID, const UniqueID objectID, const Vec2d& nPosition)
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

        void SetRotation(const UniqueID realmID, const UniqueID objectID, const double nRadians)
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

        void SetHitBoxSize(const UniqueID realmID, const UniqueID objectID, const Vec2d& nSize)
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

        Vec2d GetVelocity(const UniqueID realmID, const UniqueID objectID)
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

        Vec2d GetPosition(const UniqueID realmID, const UniqueID objectID)
        {
            if (m_realms.find(realmID) == m_realms.end()) {
                return Vec2d::Zero();
            }

            Pobject* object = m_realms.at(realmID)->GetPhysicsObject(objectID);
            if (object == nullptr) {
                return Vec2d::Zero();
            }

            return object->m_polygon->GetOrigin();
        }

        std::shared_ptr<Polygon> RemoveObject(const UniqueID realmID, const UniqueID objectID)
        {
            if (m_realms.find(realmID) == m_realms.end()) {
                return std::shared_ptr<Polygon>();
            }

            Pobject* object = m_realms.at(realmID)->GetPhysicsObject(objectID);
            if (object == nullptr) {
                return std::shared_ptr<Polygon>();
            }

            return m_realms.at(realmID)->DestroyPhysicsObject(objectID);
        }
    };

    // ================================================================================
    Physics::Impl* Physics::m_impl = nullptr;

    void Physics::Initialize()
    {
        Destroy();

        core::PtagManager::Initialize();
        m_impl = new Physics::Impl;
    }

    void Physics::Destroy()
    {
        if (m_impl != nullptr) {
            delete m_impl;
            m_impl = nullptr;
        }

        core::PtagManager::Destroy();
    }

    void Physics::OnUpdate(const float deltaTime)
    {
        m_impl->UpdateRealms(deltaTime);
    }

    RealmID Physics::CreateRealm()
    {
        return m_impl->CreateRealm();
    }

    bool Physics::RealmExists(const RealmID realm)
    {
        return m_impl->RealmExists(realm);
    }

    bool Physics::AddEntityToRealm(const RealmID realm, const Entity entity)
    {
        return false;
    }

    void Physics::ActivateRealm(const RealmID realmID)
    {
        m_impl->SetRealmActive(realmID, true);
    }

    void Physics::DeactivateRealm(const RealmID realmID)
    {
        m_impl->SetRealmActive(realmID, false);
    }

    void Physics::OnUpdateImpl(const float deltaTime)
    {
        for (auto& realmID : m_impl->m_active_realms) {
            m_impl->m_realms.at(realmID)->OnUpdate(deltaTime);
        }
    }

    void Physics::OnEndFrameImpl()
    {
        for (auto& realmID : m_impl->m_active_realms) {
            m_impl->m_realms.at(realmID)->OnEndFrame();
        }
    }


    void Physics::DeactivateRealmImpl(const UniqueID realmID)
    {
        m_impl->m_active_realms.erase(realmID);
    }

    const Polygon& Physics::GetPolygonImpl(const UniqueID realmID, const UniqueID objectID)
    {
        return m_impl->GetPolygon(realmID, objectID);
    }

    void Physics::SetObjectPropertiesImpl(const UniqueID realmID, const UniqueID objectID, PhysicalProperties nProperties)
    {
        m_impl->SetObjectProperties(realmID, objectID, nProperties);
    }

    void Physics::RemoveObjectPropertiesImpl(const UniqueID realmID, const UniqueID objectID, PhysicalProperties rProperties)
    {
        m_impl->RemoveObjectProperties(realmID, objectID, rProperties);
    }

    bool Physics::QueryObjectPropertiesImpl(const UniqueID realmID, const UniqueID objectID, PhysicalProperties propertyQuery)
    {
        return m_impl->QueryObjectProperties(realmID, objectID, propertyQuery);
    }

    UniqueID Physics::CreateObjectImpl(const UniqueID realmID, const MessageID messageID, std::shared_ptr<Polygon> polygon)
    {
        return m_impl->CreateObject(realmID, messageID, polygon);
    }

    std::shared_ptr<Polygon> Physics::RemoveObjectImpl(const UniqueID realmID, const UniqueID objectID)
    {
        return m_impl->RemoveObject(realmID, objectID);
    }

    void Physics::CreatePtagImpl(const std::string& tagName, PhysicalProperties tagProperties)
    {
        PtagManager::GetTag(PtagManager::CreateTag(tagName)).SetProperties(tagProperties);
    }

    void Physics::AddPTagImpl(const std::string& tagName, const UniqueID realmID, const UniqueID objectID)
    {
        m_impl->AddPTag(tagName, realmID, objectID);
    }

    void Physics::SetAccelerationImpl(const UniqueID realmID, const UniqueID objectID, const Vec2d& nAcceleration)
    {
        m_impl->SetAcceleration(realmID, objectID, nAcceleration);
    }

    void Physics::SetAccelerationXImpl(const UniqueID realmID, const UniqueID objectID, const double nX)
    {
        m_impl->SetAccelerationX(realmID, objectID, nX);
    }

    void Physics::SetAccelerationYImpl(const UniqueID realmID, const UniqueID objectID, const double nY)
    {
        m_impl->SetAccelerationY(realmID, objectID, nY);
    }

    void Physics::SetVelocityImpl(const UniqueID realmID, const UniqueID objectID, Vec2d& nVelocity)
    {
        m_impl->SetVelocity(realmID, objectID, nVelocity);
    }

    void Physics::SetMaximumVelocityImpl(const UniqueID realmID, const UniqueID objectID, const Vec2d& nMaxVelocity)
    {
        m_impl->SetMaximumVelocity(realmID, objectID, nMaxVelocity);
    }

    void Physics::SetGravityImpl(const UniqueID realmID, double mps2)
    {
        m_impl->SetGravity(realmID, mps2);
    }

    void Physics::SetFrictionImpl(const UniqueID realmID, const UniqueID objectID, const double friction)
    {
        m_impl->SetFriction(realmID, objectID, friction);
    }

    void Physics::SetPositionImpl(const UniqueID realmID, const UniqueID objectID, const Vec2d& nPosition)
    {
        m_impl->SetPosition(realmID, objectID, nPosition);
    }

    void Physics::SetRotationImpl(const UniqueID realmID, const UniqueID objectID, const double nRadians)
    {
        m_impl->SetRotation(realmID, objectID, nRadians);
    }

    void Physics::SetHitBoxSizeImpl(const UniqueID realmID, const UniqueID objectID, const Vec2d& nSize)
    {
        m_impl->SetHitBoxSize(realmID, objectID, nSize);
    }

    Vec2d Physics::GetVelocityImpl(const UniqueID realmID, const UniqueID objectID)
    {
        return m_impl->GetVelocity(realmID, objectID);
    }
    Vec2d Physics::GetPositionImpl(const UniqueID realmID, const UniqueID objectID)
    {
        return m_impl->GetPosition(realmID, objectID);
    }
}