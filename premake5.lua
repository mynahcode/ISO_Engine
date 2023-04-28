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

project "IsoEngine"
	location "IsoEngine"
	kind "SharedLib"
	language "C++"

	targetdir ("{prj.name}/bin/" .. outputdir .. "/%{prj.name}") -- binaries
	objdir ("{prj.name}/bin/" .. outputdir .. "/%{prj.name}")  -- intermediates

	files
	{
		"{prj.name}/src/**.h",
		"{prj.name}/src/**.cpp"
	}

	include
	{
			
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On" --statically link runtime libraries
		systemversion "latest"

		defines
		{
			"_WIN32",
			"IE_BUILD_DLL",
			"_CRT_SECURE_NO_WARNINGS"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} .. /{prj.name}/bin/" .. outputdir "/TheGame")
		}

	filter "configurations:Debug"
		defines "IE_DEBUG"
		symbols "On"

		
	filter "configurations:Release"
		defines "IE_RELEASE"
		optimize "On"

		
	filter "configurations:Dist"
		defines "IE_DIST"
		optimize "On"

	filter { "system:windows", "configurations:Release" }
		buildoptions "/MDd"

project "TheGame"
	location "TheGame"
	kind "ConsoleApp"
	language "C++"

	targetdir ("{prj.name}/bin/" .. outputdir .. "/%{prj.name}") -- binaries
	objdir ("{prj.name}/bin/" .. outputdir .. "/%{prj.name}")  -- intermediates

	files
	{
		"{prj.name}/src/**.h",
		"{prj.name}/src/**.cpp"
	}

	include
	{
		"IsoEngine\src"
	}

	links
	{
		"IsoEngine"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On" --statically link runtime libraries
		systemversion "latest"

		defines
		{
			"_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		defines "IE_DEBUG"
		symbols "On"

		
	filter "configurations:Release"
		defines "IE_RELEASE"
		optimize "On"

		
	filter "configurations:Dist"
		defines "IE_DIST"
		optimize "On"

	filter { "system:windows", "configurations:Release" }
		buildoptions "/MDd"