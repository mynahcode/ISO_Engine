-- premake5.lua for IsoEngine Solution
workspace "IsoEngine"
	architecture "x64"
	startproject "TheGame"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	
IncludeDir = {}
IncludeDir["GLAD"] = "IsoEngine/Vendor/GLAD/include"
IncludeDir["GLFW"] = "IsoEngine/Vendor/GLFW/include"
IncludeDir["ImGui"] = "IsoEngine/Vendor/imgui"
IncludeDir["glm"] = "IsoEngine/Vendor/glm"
--IncludeDir["fmt"] = "IsoEngine/Vendor/fmt"


include "IsoEngine/Vendor/GLAD"
include "IsoEngine/Vendor/GLFW"
include "IsoEngine/Vendor/ImGui"

project "IsoEngine"
	location "IsoEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on" 

	targetdir ("bin/" .. outputdir .. "/%{prj.name}") 
	objdir ("obj/" .. outputdir .. "/%{prj.name}") 

	pchheader "iepch.h"
	pchsource "IsoEngine/src/iepch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/Vendor/glm/glm",
		--"%{prj.name}/Vendor/fmt/include/fmt/**.h"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE",
		"FMT_HEADER_ONLY",
		"IE_DEBUG_MODE",
		"IMGUI_API=__declspec(dllexport)" 
	}

	includedirs
	{
		"IsoEngine/src",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		--"%{IncludeDir.fmt}"
	}

	links
	{
		"GLAD",
		"GLFW",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"_IE_PLATFORM_WINDOWS",
			"IE_BUILD_DLL"
		}

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

project "TheGame"
	location "TheGame"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("obj/" .. outputdir .. "/%{prj.name}") 

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"IsoEngine/src",
		"%{IncludeDir.glm}",
		"IsoEngine/Vendor"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"IE_DEBUG_MODE",
		"FMT_HEADER_ONLY"
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
		symbols "on"
		
	filter "configurations:Release"
		defines "IE_RELEASE"
		runtime "Release"
		optimize "on"

		
	filter "configurations:Dist"
		defines "IE_DIST"
		runtime "Release"
		optimize "on"