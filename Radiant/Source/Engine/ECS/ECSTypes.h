#pragma once
#include <cstdint>

using EntityID = std::uint32_t;
#define NO_ENTITY_ID 0

using ComponentID = std::uint32_t;
constexpr ComponentID MAX_COMPONENTS = 32;
constexpr ComponentID NOT_REGISTERED_COMPONENT = MAX_COMPONENTS + 1;
using Signature = std::bitset<MAX_COMPONENTS>;
