project "GLM"
	kind "StaticLib"
	language "C"
	architecture "x86_64"
    uuid (os.uuid("GLMUUID"))

	targetdir (thirdparty_output_dir)
    objdir (thirdparty_obj_dir)
	
	includedirs { "glm/" }

	files
	{
		"glm/glm/**"
	}
    
	filter "system:linux"
		pic "On"

		systemversion "latest"
		staticruntime "On"

		defines
		{
			"_GLM_X11"
		}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

		defines 
		{ 
			"_GLM_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"