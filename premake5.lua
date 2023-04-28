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
	cppdialect "C++17"
	staticruntime "On" 

	targetdir ("bin/" .. outputdir .. "/%{prj.name}") 
	objdir ("obj/" .. outputdir .. "/%{prj.name}") 

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"IsoEngine/src"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"_WIN32",
			"IE_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .."/TheGame")
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

	filter { "system:windows", "configurations:Debug" }
		buildoptions "/MDd"
	filter { "system:windows", "configurations:Release" }
		buildoptions "/MDd"
	filter { "system:windows", "configurations:Dist" }
		buildoptions "/MDd"

project "TheGame"
	location "TheGame"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "On"

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
			"_WIN32"
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

	filter { "system:windows", "configurations:Debug" }
		buildoptions "/MDd"
	filter { "system:windows", "configurations:Release" }
		buildoptions "/MDd"
	filter { "system:windows", "configurations:Dist" }
		buildoptions "/MDd"