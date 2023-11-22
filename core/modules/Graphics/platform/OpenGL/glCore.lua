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

        -- Include the stb_image.cpp file in compilation
        "%{IncludeDir.stb}/**.cpp"
    }

    includedirs
    {
        "src",
        "include",
        
        "%{IncludeDir.GLFW}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImGuiBackend}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}",

        "%{IncludeModule.Logger}",
        "%{IncludeDir.spdlog}",
    }

    links
    { 
        "GLFW", "GLM", "GLAD", "ImGui", md_graph["glCore"]
    }

    postbuildcommands
    {
        SendProjectDLL("Editor")
    }

    pchheader "pch.h"
    pchsource "src/pch.cpp"

    -- Need actual path to stb because tokens cannot be expanded on filter
    -- ! Issue: https://github.com/premake/premake-core/issues/1036
    filter {"files:../../../../../thirdparty/stb/**.cpp"}
        flags {"NoPCH"}

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "RDT_PLATFORM_WINDOWS",
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
    