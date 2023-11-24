-- ===============================================================================================
-- Editor Build Configuration
-- ===============================================================================================
project "glCore-Showcase"
    kind "ConsoleApp"
    language "C++"
    uuid (os.uuid("glCoreShowcaseUUID"))
	targetdir (solutionDir .. "/bin/" .. outputdir .. "/%{prj.name}")
    objdir (solutionDir .. "/bin/obj/" .. outputdir .. "/%{prj.name}")

    files
    {
        -- Editor Implementation
        "src/**.h",
        "src/**.cpp",
    }

    includedirs
    {
        "src",
        GetAllModuleIncludes('glCore-Showcase')
    }

    links
    {
        md_graph['glCore-Showcase']
    }

    postbuildcommands
    {
        GetAllDllDependencies('glCore-Showcase')
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