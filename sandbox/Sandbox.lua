-- ===============================================================================================
-- Sandbox Build Configuration
-- ===============================================================================================
project "Sandbox"
    kind "ConsoleApp"
    language "C++"
    uuid (os.uuid("SandboxUUID"))
    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin/obj/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
        "src",
        GetAllModuleIncludes('Sandbox')
    }

    links
    {
        md_graph["Sandbox"]
    }

    postbuildcommands
    {
        GetAllDllDependencies('Sandbox')
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