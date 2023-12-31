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
        "Source/**.h",
        "Source/**.cpp"
    }

    includedirs
    {
        "Source",
        
        "../Radiant/Source",
        "../Radiant/Source/Engine",

        "../Radiant/%{IncludeDir.GLFW}",
		"../Radiant/%{IncludeDir.glad}",
		"../Radiant/%{IncludeDir.ImGui}",
		"../Radiant/%{IncludeDir.ImGuiBackend}",
		"../Radiant/%{IncludeDir.glm}",
		"../Radiant/%{IncludeDir.stb}",
        "../Radiant/%{IncludeDir.openal}",
		"../Radiant/%{IncludeDir.AudioFile}",
		"../Radiant/%{IncludeDir.spdlog}",
    }

    links
    {
        "Radiant"
    }

    postbuildcommands
    {
        ("{COPY} %{cfg.targetdir}/../Radiant/Radiant.dll %{cfg.targetdir}"),
        ("{COPY} %{cfg.targetdir}/../Radiant/OpenAL32.dll %{cfg.targetdir}")
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
        staticruntime "off"
        runtime "Debug"

    filter "configurations:Release"
        defines "RDT_RELEASE"
        optimize "On"
        staticruntime "off"
        runtime "Release"