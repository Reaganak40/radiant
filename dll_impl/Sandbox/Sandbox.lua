-- ===============================================================================================
-- Sandbox Build Configuration
-- ===============================================================================================
project "Sandbox"
    kind "ConsoleApp"
    language "C++"
    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin/obj/" .. outputdir .. "/%{prj.name}")

    files
    {
        "Source/**.h",
        "Source/**.cpp"
    }

    includedirs
    {
        "../Radiant/Source",
        "../Radiant/Source/Engine",

        "../Radiant/%{IncludeDir.GLFW}",
		"../Radiant/%{IncludeDir.glad}",
		"../Radiant/%{IncludeDir.ImGui}",
		"../Radiant/%{IncludeDir.ImGuiBackend}",
		"../Radiant/%{IncludeDir.glm}",
		"../Radiant/%{IncludeDir.stb}",
		"../Radiant/%{IncludeDir.spdlog}",
    }

    links
    {
        "Radiant"
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
        defines "RDT_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "RDT_RELEASE"
        optimize "On"