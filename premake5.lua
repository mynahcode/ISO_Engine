-- premake5.lua for IsoEngine Solution
workspace "IsoEngine"
	architecture "x86_64"
	startproject "IsoEngineEditor"

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
IncludeDir["entt"] = "IsoEngine/vendor/entt/include"
IncludeDir["stb_image"] = "IsoEngine/Vendor/stb_image"
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
		"%{prj.name}/Vendor/stb_image/**.h",
		"%{prj.name}/Vendor/stb_image/**.cpp",
		"%{prj.name}/Vendor/glm/glm/**.hpp",
		"%{prj.name}/Vendor/glm/glm/**.inl",
		--"%{prj.name}/Vendor/fmt/include/fmt/**.h"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE",
		--"FMT_HEADER_ONLY",
		"_IE_DEBUG_MODE",
		"IMGUI_API=__declspec(dllexport)" 
	}

	includedirs
	{
		"IsoEngine/src",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}"
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
		"IsoEngine/Vendor",
		"%{IncludeDir.entt}"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"_IE_DEBUG_MODE",
		--"FMT_HEADER_ONLY"
	}

	links
	{
		"IsoEngine"
	}

	filter "system:windows"
		systemversion "latest"

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

project "IsoEngineEditor"
	location "IsoEngineEditor"
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
		"_IE_DEBUG_MODE",
		--"FMT_HEADER_ONLY"
	}

	links
	{
		"IsoEngine"
	}

	filter "system:windows"
		systemversion "latest"

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
