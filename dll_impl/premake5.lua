workspace "Radiant"
    architecture "x64"

    configurations 
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- ===============================================================================================
-- Radiant Engine Build Configuration
-- ===============================================================================================
project "Radiant"
    location "Radiant"
    kind "SharedLib"
    language "C++"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin/obj/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/Source/**.h",
        "%{prj.name}/Source/**.cpp"

    }
    pchheader "pch.h"
    pchsource "%{prj.name}/Source/pch.cpp"

    includedirs
    {
        "%{prj.name}/Source",
        "%{prj.name}/Vendor/spdlog/include",
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "RDT_PLATFORM_WINDOWS",
            "RDT_BUILD_DLL"
        }
    
        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }
    
    filter "configurations:Debug"
        defines "RDT_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "RDT_RELEASE"
        optimize "On"

-- ===============================================================================================
-- Sandbox Build Configuration
-- ===============================================================================================
project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin/obj/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/Source/**.h",
        "%{prj.name}/Source/**.cpp"
    }

    includedirs
    {
        "Radiant/Source",
        "Radiant/Vendor/spdlog/include",
    }

    links
    {
        "Radiant"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "RDT_PLATFORM_WINDOWS",
        }
    
    filter "configurations:Debug"
        defines "RDT_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "RDT_RELEASE"
        optimize "On"