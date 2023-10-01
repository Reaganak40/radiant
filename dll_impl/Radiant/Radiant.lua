-- ===============================================================================================
-- Radiant Engine Build Configuration
-- ===============================================================================================
project "Radiant"
    kind "SharedLib"
    language "C++"
    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin/obj/" .. outputdir .. "/%{prj.name}")

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
        "Vendor/spdlog/include/**.h",
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
		"%{IncludeDir.spdlog}",
    }

    links
    { 
        "GLFW", "GLM", "GLAD", "ImGui"
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
    
        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }
    
    filter "configurations:Debug"
        defines "RDT_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "RDT_RELEASE"
        optimize "On"