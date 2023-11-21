project "ImGui"
	kind "StaticLib"
	language "C"
	architecture "x86_64"
    uuid (os.uuid("ImGuiUUID"))

	targetdir (solutionDir .. "/bin/" .. outputdir .. "/%{prj.name}")
    objdir (solutionDir .. "/bin/obj/" .. outputdir .. "/%{prj.name}")
	
	includedirs {
		 "imgui/",
		 "%{IncludeDir.glad}",
		 "%{IncludeDir.GLFW}",
	}

	files
	{
		"imgui/*.cpp",
		"imgui/backends/imgui_impl_glfw.cpp",
		"imgui/backends/imgui_impl_opengl3.cpp"
	}

	defines 
	{
		"IMGUI_IMPL_OPENGL_LOADER_GLAD"
	}
    
	filter "system:linux"
		pic "On"

		systemversion "latest"
		staticruntime "On"

		defines
		{
			"_IMGUI_X11"
		}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

		defines 
		{ 
			"_IMGUI_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"