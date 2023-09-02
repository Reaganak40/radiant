#include "pch.h"
#include "UniqueID.h"

UniqueID GetUniqueID()
{
    return ++UUID_counter;
}
