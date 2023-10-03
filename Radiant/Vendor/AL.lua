project "AL"
	kind "StaticLib"
	language "C"
	architecture "x86_64"
    uuid ("ALUUID")

	targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../../bin/obj/" .. outputdir .. "/%{prj.name}")
	
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