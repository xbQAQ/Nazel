workspace "Nazel"
	architecture "x64"

	configurations{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Nazel"
	location "Nazel"
	kind "SharedLib"
	language "C++"
	buildoptions "/utf-8"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/ThirdParty/spdlog/include"
	}

	filter "system:Windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"NZ_PLATFORM_WINDOWS",
			"NZ_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPYFILE} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "NZ_DEBUG"
		symbols "On"
	
	filter "configurations:Release"
		defines "NZ_Release"
		optimize "On"

	filter "configurations:Dist"
		defines "NZ_Dist"
		optimize "On"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	buildoptions "/utf-8"
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
		"Nazel/ThirdParty/spdlog/include",
		"Nazel/src"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		links
		{
			"Nazel"
		}

		defines
		{
			"NZ_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "NZ_DEBUG"
		symbols "On"
	
	filter "configurations:Release"
		defines "NZ_Release"
		optimize "On"

	filter "configurations:Dist"
		defines "NZ_Dist"
		optimize "On"