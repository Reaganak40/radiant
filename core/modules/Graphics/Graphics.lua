--[[ 
    Premake for Module: Graphics

--]]

project "Graphics"
    kind "SharedLib"
    language "C++"
    uuid (os.uuid("GraphicsUUID"))
	targetdir (solutionDir .. "/bin/" .. outputdir .. "/%{prj.name}")
    objdir (solutionDir .. "/bin/obj/" .. outputdir .. "/%{prj.name}")
    ignoredefaultlibraries { "LIBCMTD" }

    files
    {
        "src/**.h",
        "src/**.cpp",
        "include/**.h",

        "platform/OpenGL/include/**.h"
    }

    includedirs
    {
        "src",
        "include",

        "platform/OpenGL/include"
    }

    links
    {
        "glCore"
    }

    postbuildcommands
    {
        SendProjectDLL("Engine")
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
            "GRAPHICS_BUILD_DLL"
        }
    
    filter "configurations:Debug"
        defines "RDT_DEBUG"
        symbols "On"
        staticruntime "off"
        runtime "Debug"

    filter "configurations:Release"
        defines "RDT_RELEASE"
        optimize "On"
        staticruntime "off"
        runtime "Release"
    