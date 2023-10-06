-- ===============================================================================================
-- Project Build Configuration
-- ===============================================================================================
project "RADIANTPROJECTNAME"
    kind "ConsoleApp"
    language "C++"
    uuid (os.uuid("RADIANTPROJECTNAMEUUID"))
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

        "RADIANTBASEDIR/Radiant/Source",
        "RADIANTBASEDIR/Radiant/Source/Engine",

        "RADIANTBASEDIR/Radiant/%{IncludeDir.GLFW}",
		"RADIANTBASEDIR/Radiant/%{IncludeDir.glad}",
		"RADIANTBASEDIR/Radiant/%{IncludeDir.ImGui}",
		"RADIANTBASEDIR/Radiant/%{IncludeDir.ImGuiBackend}",
		"RADIANTBASEDIR/Radiant/%{IncludeDir.glm}",
		"RADIANTBASEDIR/Radiant/%{IncludeDir.stb}",
        "RADIANTBASEDIR/Radiant/%{IncludeDir.openal}",
		"RADIANTBASEDIR/Radiant/%{IncludeDir.AudioFile}",
		"RADIANTBASEDIR/Radiant/%{IncludeDir.spdlog}",
    }

    links
    {
        "Radiant"
    }

    postbuildcommands
    {
        ("{COPY} %{builddir}/Radiant.dll %{cfg.targetdir}"),
        ("{COPY} %{builddir}/OpenAL32.dll %{cfg.targetdir}")
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