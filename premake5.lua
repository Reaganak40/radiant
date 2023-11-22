workspace "Radiant"
    architecture "x64"
    startproject "Editor"

    configurations 
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
solutionDir = "%{wks.location}"

-- Copies the dll from the given Project Name to the target directory of the in-scope project
function GetProjectDLL(projName)
    return ("{COPY} %{cfg.targetdir}/../" .. projName .. "/" .. projName .. ".dll %{cfg.targetdir}")
end

-- Sends the dll relative to the current project to an Application project
function SendProjectDLL(projName)
    local sendpath = ("%{wks.location}bin\\" .. outputdir .. "\\" .. projName) 
    return ("(if not exist " .. sendpath .. " mkdir " .. sendpath .. ") & {COPYFILE} %{cfg.buildtarget.relpath} " .. sendpath)
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
IncludeModule["Graphics"]  = (solutionDir .. "/core/modules/Graphics/include")
IncludeModule["glCore"]    = (solutionDir .. "/core/modules/Graphics/platform/OpenGL/include")
IncludeModule["Logger"]    = (solutionDir .. "/core/modules/Logger/include")
IncludeModule["Utilities"] = (solutionDir .. "/core/modules/Utilities/include")
IncludeModule["Engine"]    = (solutionDir .. "/core/include")

-- Module Depedency graph
md_graph = {}
md_graph["glCore"]   = {"Logger", "Utilities"}
md_graph["Utilities"]   = {}
md_graph["Graphics"] = {"glCore", "Logger", "Utilities"}
md_graph["Logger"]   = {"Utilities"}
md_graph["Engine"]   = {"Graphics", "Logger", "Utilities"}
md_graph["Editor"]   = {"Engine"}

-- Queue structure
function Queue()
    out = {}
    local first, last = 0, -1
    out.push = function(item)
        last = last + 1
        out[last] = item
      end
    out.pop = function()
        if first <= last then
          local value = out[first]
          out[first] = nil
          first = first + 1
          return value
        end
      end
      return out
    end

-- Reads the Module Dependency graph (should be a DAG), and returns a list of all child nodes.
function GetModuleDependencies(projName)
    local unique_models = {}
    if md_graph[projName] == nil then
        return unique_models
    end
    local queue = Queue()
    queue.push(projName)

    local count = 1
    while count > 0 do
        local parent = queue.pop()
        count = count - 1
        for idx, child_module in pairs(md_graph[parent]) do
            if unique_models[child_module] == nil then
                unique_models[child_module] = true
                queue.push(child_module)
                count = count + 1
            end
        end
    end
    return unique_models
end

function GetAllDllDependencies(projName)
    local copy_commands = {}
    for dll_module, _ in pairs(GetModuleDependencies(projName)) do
        table.insert(copy_commands, GetProjectDLL(dll_module))
    end
    return copy_commands
end

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