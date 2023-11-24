-- ===============================================================================================
-- Editor Build Configuration
-- ===============================================================================================
project "Editor"
    kind "SharedLib"
    language "C++"
    uuid (os.uuid("EditorUUID"))
	targetdir (solutionDir .. "/bin/" .. outputdir .. "/%{prj.name}")
    objdir (solutionDir .. "/bin/obj/" .. outputdir .. "/%{prj.name}")

    files
    {
        -- Editor Implementation
        "src/**.h",
        "src/**.cpp",
        "include/**.h",
    }

    includedirs
    {
        "include",
        "src",
        GetAllModuleIncludes('Editor')
    }

    links
    {
        md_graph["Editor"]
    }

    postbuildcommands
    {
        SendProjectDLL("Sandbox")
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "RDT_PLATFORM_WINDOWS",
            "EDITOR_BUILD_DLL"
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