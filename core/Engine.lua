-- ===============================================================================================
-- Radiant Engine Build Configuration
-- ===============================================================================================

project "Engine"
    kind "SharedLib"
    language "C++"
    uuid (os.uuid("EngineUUID"))
	targetdir (solutionDir .. "/bin/" .. outputdir .. "/%{prj.name}")
    objdir (solutionDir .. "/bin/obj/" .. outputdir .. "/%{prj.name}")
    ignoredefaultlibraries { "LIBCMTD" }

    files
    {
        "src/**.h",
        "src/**.cpp",

        ("%{IncludeModule.Graphics}" .. "/**.h"),


    }

    includedirs
    {
        "src",

        "%{IncludeModule.Graphics}"
    }

    links
    {
        
    }

    postbuildcommands
    {
        CopyDLL("Editor")
    }

    pchheader "pch.h"
    pchsource "Source/pch.cpp"

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "RDT_PLATFORM_WINDOWS",
            "RDT_BUILD_DLL"
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
    