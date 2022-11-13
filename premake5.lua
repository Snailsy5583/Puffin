workspace "Puffin"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Puffin"
	location "Puffin"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include"
	}

	filter "system:windows"
		cppdialect "c++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"PF_PLATFORM_WINDOWS",
			"PF_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "PF_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "PF_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "PF_DIST"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Puffin/vendor/spdlog/include",
		"Puffin/src"
	}

	links
	{
		"Puffin"
	}

	filter "system:windows"
		cppdialect "c++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"PF_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "PF_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "PF_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "PF_DIST"
		optimize "On"