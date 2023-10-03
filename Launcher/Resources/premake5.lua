workspace "RADIANTPROJECTNAME"
    architecture "x64"
    startproject "RADIANTPROJECTNAME"

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
    externalproject "GLFW"
        location "RADIANTBASEDIR/Radiant/Vendor"
        uuid ("GLFWUUID")
        kind "StaticLib"
        language "C"
    
    externalproject "GLAD"
        location "RADIANTBASEDIR/Radiant/Vendor"
        uuid ("GLADUUID")
        kind "StaticLib"
        language "C"

    externalproject "GLM"
        location "RADIANTBASEDIR/Radiant/Vendor"
        uuid ("GLMUUID")
        kind "StaticLib"
        language "C"

    externalproject "ImGui"
        location "RADIANTBASEDIR/Radiant/Vendor"
        uuid ("ImGuiUUID")
        kind "StaticLib"
        language "C"

    externalproject "ImGui"
        location "RADIANTBASEDIR/Radiant/Vendor"
        uuid ("ALUUID")
        kind "StaticLib"
        language "C"
group ""

externalproject "Radiant"
    location "RADIANTBASEDIR/Radiant"
    uuid ("RadiantUUID")
    kind "SharedLib"
    language "C++"

include "RADIANTPROJECTNAME/RADIANTPROJECTNAME.lua"