--[[ 
    Premake for Module: Audio
--]]

project "Audio"
    kind "SharedLib"
    language "C++"
    uuid (os.uuid("AudioUUID"))
	targetdir (radiant_output_dir)
    objdir (radiant_obj_dir)
    ignoredefaultlibraries { "LIBCMTD" }

    files
    {
        "**.h",
        "**.hpp",
        "**.cpp",
    }

    includedirs
    {
        radiant_public_headers,
        radiant_private_headers
    }

    links
    {
        md_graph["Audio"]
    }

    postbuildcommands
    {
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
            "RDT_BUILD_DLL"
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
    