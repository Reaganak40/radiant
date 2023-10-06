-- ===============================================================================================
-- Radiant Engine Build Configuration
-- ===============================================================================================

project "Radiant"
    kind "SharedLib"
    language "C++"
    uuid (os.uuid("RadiantUUID"))
    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin/obj/" .. outputdir .. "/%{prj.name}")
    ignoredefaultlibraries { "LIBCMTD" }

    files
    {
        "Source/**.h",
        "Source/**.cpp",

        "Vendor/glfw/include/**.h",
        "Vendor/glad/include/**.h",
        "Vendor/glm/**.h",
        "Vendor/imgui/**.h",
        "Vendor/imgui/backends/**.h",
        "Vendor/stb/**.h",
        "Vendor/stb/**.cpp",
        "Vendor/AL/**.h",
        "Vendor/AudioFile-1.1.1/**.h",
        "Vendor/spdlog/include/spdlog/spdlog.h",
        "Vendor/spdlog/include/spdlog/sinks/stdout_color_sinks.h",
    }

    includedirs
    {
        "Source",
        "Source/Engine",
        
        "%{IncludeDir.GLFW}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImGuiBackend}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.openal}",
		"%{IncludeDir.AudioFile}",
		"%{IncludeDir.spdlog}",
    }

    links
    { 
        "GLFW", "GLM", "GLAD", "ImGui"
    }

    postbuildcommands
    {
        ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
    }

    pchheader "pch.h"
    pchsource "Source/pch.cpp"
    filter {"files:Vendor/**.cpp"}
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
        defines "RDT_DEBUG"
        symbols "On"
        staticruntime "off"
        runtime "Debug"

    filter "configurations:Release"
        defines "RDT_RELEASE"
        optimize "On"
        staticruntime "off"
        runtime "Release"
    