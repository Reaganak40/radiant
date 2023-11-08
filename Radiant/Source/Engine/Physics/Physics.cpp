#include "pch.h"
#include "Physics.h"
#include "Ptag.h"
#include "Collider.h"
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
                realm.Flush();
            }
        }
    };

    // ================================================================================
    Physics::Impl* Physics::m_impl = nullptr;

    void Physics::Initialize()
    {
        Destroy();

        core::PtagManager::Initialize();
        ColliderManager::Initialize();

        m_impl = new Physics::Impl;
    }

    void Physics::Destroy()
    {
        if (m_impl != nullptr) {
            delete m_impl;
            m_impl = nullptr;
        }

        ColliderManager::Destroy();
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
        return m_impl->AddEntityToRealm(realm, entity);
    }

    void Physics::ActivateRealm(const RealmID realmID)
    {
        m_impl->SetRealmActive(realmID, true);
    }

    void Physics::DeactivateRealm(const RealmID realmID)
    {
        m_impl->SetRealmActive(realmID, false);
    }
}