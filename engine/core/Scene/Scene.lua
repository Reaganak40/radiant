--[[ 
    Premake for Module: Scene

--]]


project "Scene"
    kind "SharedLib"
    language "C++"
    uuid (os.uuid("SceneUUID"))
	targetdir (solutionDir .. "/bin/" .. outputdir .. "/%{prj.name}")
    objdir (solutionDir .. "/bin/obj/" .. outputdir .. "/%{prj.name}")
    ignoredefaultlibraries { "LIBCMTD" }

    files
    {
        "src/**.h",
        "src/**.cpp",
        "include/**.h",
    }

    includedirs
    {
        "src",
        "include",
        GetAllModuleIncludes('Scene')
    }

    links
    {
        md_graph["Scene"]
    }

    postbuildcommands
    {
        SendProjectDLL("Sandbox")
    }

    pchheader "pch.h"
    pchsource "src/pch.cpp"

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "RDT_PLATFORM_WINDOWS",
            "SCENE_BUILD_DLL"
        }
    
    filter "configurations:Debug"
        symbols "On"
        staticruntime "off"
        runtime "Debug"
        defines
        {
            "RDT_DEBUG",
            "RDT_USE_EDITOR"
        }   

    filter "configurations:Release"
        optimize "On"
        staticruntime "off"
        runtime "Release"
        defines
        {
            "RDT_RELEASE",
            "RDT_USE_EDITOR"
        }

    filter "configurations:Publish"
        optimize "On"
        staticruntime "off"
        runtime "Release"
        defines
        {
            "RDT_RELEASE",
        }
    