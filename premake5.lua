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
IncludeDir["entt"] = "IsoEngine/Vendor/entt/include"
IncludeDir["stb_image"] = "IsoEngine/Vendor/stb_image"
IncludeDir["fmt"] = "IsoEngine/Vendor/fmt"
IncludeDir["yaml_cpp"] = "IsoEngine/Vendor/yaml-cpp/include"


include "IsoEngine/Vendor/GLAD"
include "IsoEngine/Vendor/GLFW"
include "IsoEngine/Vendor/ImGui"
include "IsoEngine/Vendor/yaml-cpp"

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
		"%{prj.name}/Vendor/entt/include/**.hpp",
		"%{prj.name}/Vendor/fmt/include/fmt/core.h",
		"%{prj.name}/Vendor/fmt/include/fmt/format.h",
		"%{prj.name}/Vendor/fmt/include/fmt/format-inl.h",
		"%{prj.name}/Vendor/fmt/src/format.cc"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE",
		"FMT_HEADER_ONLY",
		"_IE_DEBUG_MODE",
		"IMGUI_API=__declspec(dllexport)",
		"YAML_CPP_STATIC_DEFINE"
	}

	includedirs
	{
		"IsoEngine/src",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.fmt}",
		"%{IncludeDir.yaml_cpp}"
	}

	links
	{
		"GLAD",
		"GLFW",
		"ImGui",
		"yaml-cpp",
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
		"%{IncludeDir.entt}",
		"%{IncludeDir.fmt}"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"_IE_DEBUG_MODE",
		"FMT_HEADER_ONLY",
		"YAML_CPP_STATIC_DEFINE"
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
		"IsoEngine/Vendor",
		"%{IncludeDir.entt}",
		"%{IncludeDir.fmt}"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"_IE_DEBUG_MODE",
		"FMT_HEADER_ONLY",
		"YAML_CPP_STATIC_DEFINE"
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
