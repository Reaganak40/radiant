--[[ 
    Premake for Module: OpenGL
--]]

project_name = "OpenGL"

project "OpenGL"
    kind "SharedLib"
    language "C++"
    uuid (os.uuid("OpenGLUUID"))
	targetdir (radiant_output_dir)
    objdir (radiant_obj_dir)
    ignoredefaultlibraries { "LIBCMTD" }


    files 
    { 
        GetModuleFiles(project_name),

        -- Include the stb_image.cpp file in compilation
        "%{tp_include.stb}/**.cpp"
     }
    includedirs { GetModuleIncludes(project_name) }
    links { GetModuleLinks(project_name) }

    postbuildcommands
    {
    }

    pchheader "pch.h"
    pchsource "pch.cpp"

    -- Need actual path to stb because tokens cannot be expanded on filter
    -- ! Issue: https://github.com/premake/premake-core/issues/1036
    filter {"files: ../../../../../thirdparty/stb/**.cpp"}
        flags {"NoPCH"}

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
            "RDT_PUBLISH",
        }
    