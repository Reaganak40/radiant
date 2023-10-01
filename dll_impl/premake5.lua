workspace "Radiant"
    architecture "x64"
    startproject "Sandbox"

    configurations 
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to Radiant Project
IncludeDir = {}
IncludeDir["GLFW"] = "Vendor/glfw/include"
IncludeDir["glad"] = "Vendor/glad/include"
IncludeDir["ImGui"] = "Vendor/imgui"
IncludeDir["ImGuiBackend"] = "Vendor/imgui/backends"
IncludeDir["glm"] = "Vendor/glm"
IncludeDir["stb"] = "Vendor/stb"
IncludeDir["openal"] = "Vendor/AL"
IncludeDir["AudioFile"] = "Vendor/AudioFile-1.1.1"
IncludeDir["spdlog"] = "Vendor/spdlog/include"

-- Projects
group "Dependencies"
	include "Radiant/Vendor/glfw.lua"
	include "Radiant/Vendor/glad.lua"
	include "Radiant/Vendor/glm.lua"
	include "Radiant/Vendor/imgui.lua"
group ""

include "Radiant/Radiant.lua"
include "Sandbox/Sandbox.lua"