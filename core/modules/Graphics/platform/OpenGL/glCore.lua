-- ===============================================================================================
-- glCore Build Configuration
-- ===============================================================================================

project "glCore"
    kind "SharedLib"
    language "C++"
    uuid (os.uuid("glCoreUUID"))
	targetdir (solutionDir .. "/bin/" .. outputdir .. "/%{prj.name}")
    objdir (solutionDir .. "/bin/obj/" .. outputdir .. "/%{prj.name}")
    ignoredefaultlibraries { "LIBCMTD" }

    files
    {
        -- Implementation Files
        "src/**.h",
        "src/**.cpp",
        "include/**.h",
    }

    includedirs
    {
        "src",
        
        "%{IncludeDir.GLFW}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImGuiBackend}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}",
    }

    links
    { 
        "GLFW", "GLM", "GLAD", "ImGui"
    }

    postbuildcommands
    {
        SendProjectDLL("Graphics")
    }

    pchheader "pch.h"
    pchsource "src/pch.cpp"
    filter {"files:vendor/**.cpp"}
        flags {"NoPCH"}

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "GL_CORE_PLATFORM_WINDOWS",
            "GL_CORE_BUILD_DLL"
        }
    
    filter "configurations:Debug"
        defines "GL_CORE_DEBUG"
        symbols "On"
        staticruntime "off"
        runtime "Debug"

    filter "configurations:Release"
        defines "GL_CORE_RELEASE"
        optimize "On"
        staticruntime "off"
        runtime "Release"
    