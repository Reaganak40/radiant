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
function GetProjectDLL(projName)
    return ("{COPY} %{cfg.targetdir}/../" .. projName .. "/" .. projName .. ".dll %{cfg.targetdir}")
end

function SendProjectDLL(projName)
    local sendpath = ("%{wks.location}bin\\" .. outputdir .. "\\" .. projName) 
    return ("if not exist " .. sendpath .. " mkdir " .. sendpath .. " & {COPYFILE} %{cfg.buildtarget.relpath} " .. sendpath)
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

-- Radiant Module include directories
IncludeModule = {}
IncludeModule["Graphics"] = (solutionDir .. "/core/modules/Graphics/include")
IncludeModule["Logger"] = (solutionDir .. "/core/modules/Logger/include")
IncludeModule["Utilities"] = (solutionDir .. "/core/modules/Utilities/include")


-- Projects
group "Third-Party"
include "thirdparty/glfw/glfw.lua"
include "thirdparty/glad/glad.lua"
include "thirdparty/glm/glm.lua"
include "thirdparty/imgui/imgui.lua"
include "thirdparty/AL/AL.lua"
group ""

group "Core-Modules"
    include "core/modules/Utilities/Utilities.lua"
    include "core/modules/Logger/Logger.lua"
    include "core/modules/Graphics/platform/OpenGL/glCore.lua"
    include "core/modules/Graphics/Graphics.lua"
group ""

include "core/Engine.lua"
include "editor/Editor.lua"
--include "sandbox/Sandbox.lua"