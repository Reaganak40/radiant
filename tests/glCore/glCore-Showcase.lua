-- ===============================================================================================
-- Editor Build Configuration
-- ===============================================================================================
project "glCore-Showcase"
    kind "ConsoleApp"
    language "C++"
    uuid (os.uuid("glCoreShowcaseUUID"))
	targetdir (test_output_dir)
    objdir (test_obj_dir)

    files
    {
        -- Editor Implementation
        "src/**.h",
        "src/**.cpp",
    }

    includedirs
    {
        "src",
        radiant_public_headers,
        radiant_private_headers
    }

    links
    {
        md_graph['glCore-Showcase']
    }

    postbuildcommands
    {
        GetProjectDLL("OpenGL"),
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