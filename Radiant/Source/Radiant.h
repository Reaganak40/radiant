#pragma once
/*
	Header file to be included by Radiant Applications
*/
#include "Engine/Application.h"
#include "Engine/Logging/Log.h"

// -- Radiant Engine APIs --------------------
#include "Engine/Graphics/Renderer.h"
#include "Engine/Messaging/MessageBus.h"
#include "Engine/Messaging/MessageTypes.h"
#include "Engine/Physics/Physics.h"
#include "Engine/Utils/Utils.h"
#include "Engine/Utils/Input.h"
#include "Engine/Utils/Timestep.h"
// -------------------------------------------

// -- Entity Component System ----------------
#include "Engine/ECS/ECS.h"
#include "Engine/Physics/PhysicsSystem.h"
#include "Engine/Graphics/RenderSystem.h"
// -------------------------------------------

// -- Game Developer APIs --------------------
#include "Editor/Editor.h"
// -------------------------------------------

// -- Meaningless Warnings -------------------
#pragma warning ( disable: 4065 )
// -------------------------------------------

