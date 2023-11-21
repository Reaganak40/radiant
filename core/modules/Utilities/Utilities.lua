--[[ 
    Premake for Module: Utilities

--]]


project "Utilities"
    kind "SharedLib"
    language "C++"
    uuid (os.uuid("UtilitiesUUID"))
	targetdir (solutionDir .. "/bin/" .. outputdir .. "/%{prj.name}")
    objdir (solutionDir .. "/bin/obj/" .. outputdir .. "/%{prj.name}")
    ignoredefaultlibraries { "LIBCMTD" }

    files
    {
        "src/**.h",
        "src/**.cpp",
        "include/utils_api.h",
    }

    includedirs
    {
        "src",
        "include",

        "%{IncludeModule.Utilities}",
    }

    links
    {
    }

    postbuildcommands
    {
        SendProjectDLL("Engine"),
        SendProjectDLL("glCore"),
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
            "UTILS_BUILD_DLL"
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
    