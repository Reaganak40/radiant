--[[ 
    Premake for Module: Logger

--]]


project "Logger"
    kind "SharedLib"
    language "C++"
    uuid (os.uuid("LoggerUUID"))
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
        
        "%{IncludeDir.spdlog}",
        "%{IncludeModule.Utilities}",
    }

    links
    {
        "Utilities"
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
            "LOGGER_BUILD_DLL"
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
    