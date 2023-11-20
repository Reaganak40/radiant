-- ===============================================================================================
-- Editor Build Configuration
-- ===============================================================================================
project "Editor"
    kind "ConsoleApp"
    language "C++"
    uuid (os.uuid("EditorUUID"))
	targetdir (solutionDir .. "/bin/" .. outputdir .. "/%{prj.name}")
    objdir (solutionDir .. "/bin/obj/" .. outputdir .. "/%{prj.name}")

    files
    {
        -- Editor Implementation
        "src/**.h",
        "src/**.cpp",

        -- Radiant Engine Files
        (solutionDir .. "/core/include/**.h")
    }

    includedirs
    {
        "src",
        (solutionDir .. "/core/include")
    }

    links
    {
        "Engine"
    }

    postbuildcommands
    {
        CopyDLL("Engine")
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
        staticruntime "off"
        runtime "Debug"

    filter "configurations:Release"
        defines "RDT_RELEASE"
        optimize "On"
        staticruntime "off"
        runtime "Release"