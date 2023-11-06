/*******************************************************************
*	Module:  ECS
*	File:    ECSTypes.h
*
*	Author: Reagan Kelley
*
*   The file contains the core data types of the entity component
*	system, and is used by the Entity, Component, and System
*	managers.
*******************************************************************/
#pragma once
#include <cstdint>

using Entity = std::uint32_t;
#define NO_ENTITY_ID 0
using ComponentID = std::uint32_t;
constexpr ComponentID MAX_COMPONENTS = 32;
constexpr ComponentID NOT_REGISTERED_COMPONENT = MAX_COMPONENTS + 1;
using Signature = std::bitset<MAX_COMPONENTS>;