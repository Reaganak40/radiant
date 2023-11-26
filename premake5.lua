--[[
    MODULE DEPENDENCY GRAPH
    Defines the information flow of the modules

    Key: Module Name
    Val: The modules used in its implementation
--]]
md_graph                = {}

-- public modules
md_graph["Utils"]       = {}
md_graph["Window"]      = {"Utils"}
md_graph["Graphics"]    = {"Utils"}
md_graph["Physics"]     = {"Utils"}
md_graph["Audio"]       = {"Utils"}
md_graph["ECS"]         = {"Graphics", "Physics", "Audio", "Utils"}
md_graph["Scene"]       = {"ECS", "Utils"}
md_graph["Editor"]      = {"Scene", "ECS", "Audio", "Physics", "Graphics", "Window", "Utils"}
md_graph["Application"] = {"Editor", "Scene", "Window"}

-- core/private modules
md_graph["Logger"]      = {"Utils"}
md_graph["OpenGL"]      = {"Utils"}

-- Test Modules
md_graph['glCore-Showcase'] = {"OpenGL", "Utils"}

--[[
    THIRD-PARTY MODULE INCLUDES
    Defines the third party include paths

    Key: third party name
    Val: path to files from solution dir
--]]
tp_include = {}
tp_include["GLFW"]          = "%{wks.location}/thirdparty/glfw/glfw/include"
tp_include["glad"]          = "%{wks.location}/thirdparty/glad/glad/include"
tp_include["ImGui"]         = "%{wks.location}/thirdparty/imgui/imgui"
tp_include["ImGuiBackend"]  = "%{wks.location}/thirdparty/imgui/imgui/backends"
tp_include["glm"]           = "%{wks.location}/thirdparty/glm/glm"
tp_include["stb"]           = "%{wks.location}/thirdparty/stb"
tp_include["openal"]        = "%{wks.location}/thirdparty/AL"
tp_include["AudioFile"]     = "%{wks.location}/thirdparty/AudioFile-1.1.1"
tp_include["spdlog"]        = "%{wks.location}/thirdparty/spdlog/spdlog/include"

--[[
    Module third party dependencies
    Defines what third party libaries a given module references

    Key: Module name
    Val: third party libraries it depends on
    NOTE: This only includes third party vs projects
--]]
mtpd = {}
mtpd["Utils"]       = {}
mtpd["Window"]      = {}
mtpd["Graphics"]    = {"GLM"}
mtpd["Physics"]     = {}
mtpd["Audio"]       = {"openal"}
mtpd["ECS"]         = {}
mtpd["Scene"]       = {}
mtpd["Editor"]      = {"ImGui"}
mtpd["Application"] = {}
mtpd["Logger"]      = {"spdlog"}
mtpd["OpenGL"]      = {"GLFW", "GLM", "GLAD", "ImGui"}

outputFolder = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

radiant_output_dir = ("{%wks.location}/engine/bin/" .. outputFolder .. "/%{prj.name}")
radiant_obj_dir = ("{%wks.location}/engine/bin/obj/" .. outputFolder .. "/%{prj.name}")

thirdparty_output_dir = ("%{wks.location}/thirdparty/bin/" .. outputFolder .. "/%{prj.name}")
thirdparty_obj_dir = ("%{wks.location}/thirdparty/bin/obj/" .. outputFolder .. "/%{prj.name}")

sandbox_output_dir = ("%{wks.location}/bin/" .. outputFolder .. "/%{prj.name}")
sandbox_obj_dir = ("%{wks.location}/bin/obj/" .. outputFolder .. "/%{prj.name}")

test_output_dir = ("%{wks.location}/tests/bin/" .. outputFolder .. "/%{prj.name}")
test_obj_dir = ("%{wks.location}/tests/bin/obj/" .. outputFolder .. "/%{prj.name}")

radiant_public_headers = "%{wks.location}/engine/include"
radiant_private_headers = "%{wks.location}/engine/src/Core/include"

-- Copies the dll from the given Project Name to the target directory of the in-scope project
function GetProjectDLL(projName)
    return ("{COPY} {%wks.location}/engine/bin/" .. outputFolder .. "/" .. projName .. ".dll %{cfg.targetdir}")
end

function GetProjectLocation(projName)
    if md_graph[projName] == nil then
        error("Undefined project:", projName)
    end

    if projName == "OpenGL" or projName == "Logger" then
        return ("%{wks.location}/engine/src/Core/src/" .. projName)
    end
    
    return ("%{wks.location}/engine/src/" .. projName)
end

workspace "Radiant"
    architecture "x64"
    startproject "Sandbox"

    configurations 
    {
        "Debug",
        "Release",
        "Publish",
    }

-- Visual Studio Projects
group "Third-Party"
    include "thirdparty/glfw/glfw.lua"
    include "thirdparty/glad/glad.lua"
    include "thirdparty/glm/glm.lua"
    include "thirdparty/imgui/imgui.lua"
    include "thirdparty/AL/AL.lua"
group ""

group "Engine"
    include "engine/src/Utils/Utils.lua"
    include "engine/src/Window/Window.lua"
    include "engine/src/Graphics/Graphics.lua"
    include "engine/src/Physics/Physics.lua"
    include "engine/src/Audio/Audio.lua"
    include "engine/src/ECS/ECS.lua"
    include "engine/src/Scene/Scene.lua"
    include "engine/src/Editor/Editor.lua"
    include "engine/src/Application/Application.lua"
group ""

group "Engine/Core"
    include "engine/src/Core/src/OpenGL/OpenGL.lua"
    include "engine/src/Core/src/Logger/Logger.lua" 
group ""
    
group "Tests"
include "tests/glCore/glCore-Showcase.lua"
group ""
    
include "sandbox/Sandbox.lua"