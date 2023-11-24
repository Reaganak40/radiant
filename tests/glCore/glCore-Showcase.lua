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
        defines "RDT_DEBUG"
        symbols "On"
        staticruntime "off"
        runtime "Debug"

    filter "configurations:Release"
        defines "RDT_RELEASE"
        optimize "On"
        staticruntime "off"
        runtime "Release"