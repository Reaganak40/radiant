project "AL"
	kind "StaticLib"
	language "C"
	architecture "x86_64"
    uuid (os.uuid("ALUUID"))

	targetdir (thirdparty_output_dir)
    objdir (thirdparty_obj_dir)
	
	includedirs { "AL/" }

	files
	{
		"AL/**"
	}
    
	filter "system:linux"
		pic "On"

		systemversion "latest"
		staticruntime "On"

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"


	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"