-- premake5.lua for IsoEngine Solution
workspace "IsoEngine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	
IncludeDir = {}
IncludeDir["GLFW"] = "IsoEngine/vendor/GLFW/include"

project "IsoEngine"
	location "IsoEngine"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off" 

	targetdir ("bin/" .. outputdir .. "/%{prj.name}") 
	objdir ("obj/" .. outputdir .. "/%{prj.name}") 

	pchheader "iepch.h"
	pchsource "IsoEngine/src/iepch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	includedirs
	{
		"./IsoEngine/src",
		"%{IncludeDir.GLFW}"
	}

	links
	{
		"GLFW"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"_IE_PLATFORM_WINDOWS",
			"IE_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .."/TheGame")
		}

	filter "configurations:Debug"
		defines "IE_DEBUG"
		runtime "Debug"
		buildoptions "/MDd"
		symbols "On"
		
	filter "configurations:Release"
		defines "IE_RELEASE"
		runtime "Release"
		buildoptions "/MD"
		optimize "On"

		
	filter "configurations:Dist"
		defines "IE_DIST"
		runtime "Release"
		buildoptions "/MD"
		optimize "On"

project "TheGame"
	location "TheGame"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("obj/" .. outputdir .. "/%{prj.name}") 

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"IsoEngine/src"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	links
	{
		"IsoEngine"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"_IE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "IE_DEBUG"
		runtime "Debug"
		buildoptions "/MDd"
		symbols "On"
		
	filter "configurations:Release"
		defines "IE_RELEASE"
		runtime "Release"
		buildoptions "/MD"
		optimize "On"

		
	filter "configurations:Dist"
		defines "IE_DIST"
		runtime "Release"
		buildoptions "/MD"
		optimize "On"