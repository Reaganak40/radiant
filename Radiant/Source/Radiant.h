#pragma once
/*
	Header file to be included by Radiant Applications
*/

// -- Needs includes from pch ----------------
#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>
#include <memory>

// -- Application ----------------------------
#include "Engine/Application.h"

// -- Radiant Engine APIs --------------------
#include "Engine/Logging/Log.h"
#include "Engine/Graphics/Renderer.h"
#include "Engine/Graphics/Model.h"
#include "Engine/Graphics/Texture/TextureManager.h"
#include "Engine/Messaging/MessageBus.h"
#include "Engine/Messaging/MessageTypes.h"
#include "Engine/Physics/Physics.h"
#include "Engine/Physics/Collider.h"
#include "Engine/Utils/Utils.h"
#include "Engine/Utils/Input.h"
#include "Engine/Utils/Timestep.h"
#include "Engine/Scene/Scene.h"
#include "Engine/Scene/Layer.h"

// -- Entity Component System ----------------
#include "Engine/ECS/ECS.h"
#include "Engine/Physics/PhysicsSystem.h"
#include "Engine/Graphics/RenderSystem.h"
// -------------------------------------------

// -- Game Developer APIs --------------------
#include "Editor/Editor.h"

// -- Meaningless Warnings -------------------
#pragma warning ( disable: 4065 )
// -------------------------------------------

