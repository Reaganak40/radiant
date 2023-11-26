-- ===============================================================================================
-- Sandbox Build Configuration
-- ===============================================================================================
project "Sandbox"
    kind "ConsoleApp"
    language "C++"
    uuid (os.uuid("SandboxUUID"))
    targetdir (sandbox_output_dir)
    objdir (sandbox_obj_dir)

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
        "src",
        radiant_public_headers,
    }

    links
    {
    }

    postbuildcommands
    {
        GetProjectDLL("Utils"),
        GetProjectDLL("Window"),
        GetProjectDLL("Graphics"),
        GetProjectDLL("Physics"),
        GetProjectDLL("Audio"),
        GetProjectDLL("ECS"),
        GetProjectDLL("Scene"),
        GetProjectDLL("Editor"),
        GetProjectDLL("Application"),
        GetProjectDLL("Logger"),
        GetProjectDLL("OpenGL"),
        GetProjectDLL("Engine")      
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
        kind "WindowedApp"
        entrypoint "mainCRTStartup"
        defines
        {
            "RDT_RELEASE",
        }