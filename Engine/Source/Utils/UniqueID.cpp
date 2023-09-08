#include "pch.h"
#include "UniqueID.h"

namespace Radiant {

    std::vector<UniqueID> UUID_Handler::freeIDs = {};

    UniqueID GetUniqueID()
    {
        UniqueID UUID = UUID_Handler::GetID();
        printf("ID given: %d\n", UUID == NO_UUID ? UUID_counter + 1 : UUID);
        return UUID == NO_UUID ? ++UUID_counter : UUID;
    }

    void FreeUniqueID(UniqueID UUID)
    {
        UUID_Handler::AddID(UUID);
    }

    void UUID_Handler::AddID(UniqueID UUID)
    {
        freeIDs.push_back(UUID);
    }

    UniqueID UUID_Handler::GetID()
    {
        UniqueID res = NO_UUID;
        if (freeIDs.size()) {
            res = freeIDs.back();
            freeIDs.pop_back();
        }
        return res;
    }

}