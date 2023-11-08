project "yaml-cpp"
	kind "StaticLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}") 
	objdir ("obj/" .. outputdir .. "/%{prj.name}") 

	files
	{
		"src/**.h",
		"src/**.cpp",

		"include/**.h",
		"include/node/**.h"
	}

	includedirs
	{
		"include",
		"include/node"
	}

	defines
	{
		"YAML_CPP_STATIC_DEFINE"
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "on"

	filter "system:linux"
		pic "on"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "on"

	filter "configurations:Debug"
		defines "IE_DEBUG"
		runtime "Debug"
		symbols "on"
		
	filter "configurations:Release"
		defines "IE_RELEASE"
		runtime "Release"
		optimize "on"

		
	filter "configurations:Dist"
		defines "IE_DIST"
		runtime "Release"
		optimize "on"
