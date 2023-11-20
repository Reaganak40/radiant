workspace "Radiant"
    architecture "x64"
    startproject "Editor"

    configurations 
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
solutionDir = path.getabsolute("")

-- Copies the dll from the given Project Name to the target directory of the in-scope project
function CopyDLL(projName)
    return ("{COPY} %{cfg.targetdir}/../" .. projName .. "/" .. projName .. ".dll %{cfg.targetdir}")
end

-- All include directories
IncludeDir = {}
IncludeDir["GLFW"]          = (solutionDir .. "/thirdparty/glfw/glfw/include")
IncludeDir["glad"]          = (solutionDir .. "/thirdparty/glad/glad/include")
IncludeDir["ImGui"]         = (solutionDir .. "/thirdparty/imgui/imgui")
IncludeDir["ImGuiBackend"]  = (solutionDir .. "/thirdparty/imgui/imgui/backends")
IncludeDir["glm"]           = (solutionDir .. "/thirdparty/glm/glm")
IncludeDir["stb"]           = (solutionDir .. "/thirdparty/stb")
IncludeDir["openal"]        = (solutionDir .. "/thirdparty/AL")
IncludeDir["AudioFile"]     = (solutionDir .. "/thirdparty/AudioFile-1.1.1")
IncludeDir["spdlog"]        = (solutionDir .. "/thirdparty/spdlog/spdlog/include")

IncludeModule["Graphics"] = (solutionDir .. "core/modules/graphics/include")

-- Projects
group "Third-Party"
	include "thirdparty/glfw/glfw.lua"
	include "thirdparty/glad/glad.lua"
	include "thirdparty/glm/glm.lua"
	include "thirdparty/imgui/imgui.lua"
    include "thirdparty/AL/AL.lua"
group ""

group "Core-Modules"
    include "core/modules/Graphics/platform/glCore.lua"
    include "core/modules/Graphics/Graphics.lua"
group ""

include "core/Engine.lua"
include "editor/Editor.lua"
--include "sandbox/Sandbox.lua"