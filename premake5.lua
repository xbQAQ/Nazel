workspace "Nazel"    -- 解决方案名称
	architecture "x64"

	configurations{
		"Debug",
		"Release",
		"Dist"
	}

	-- 启动项目
	startproject "Sandbox"

	-- 启用多处理器编译
	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- 包含目录相对路径
IncludeDir = {}
IncludeDir["GLFW"] = "Nazel/ThirdParty/GLFW/include"
IncludeDir["Glad"] = "Nazel/ThirdParty/Glad/include"
IncludeDir["ImGui"] = "Nazel/ThirdParty/imgui"
IncludeDir["glm"] = "Nazel/ThirdParty/glm"
IncludeDir["stb_image"] = "Nazel/ThirdParty/stb_image"

include "Nazel/ThirdParty/GLFW"
include "Nazel/ThirdParty/Glad"
include "Nazel/ThirdParty/imgui"

project "Nazel"        -- Nazel项目
	location "Nazel"    -- 在sln文件目录下的Nazel文件夹
	kind "StaticLib"    -- 静态库
	language "C++"
	buildoptions "/utf-8"
	staticruntime "on"    -- 静态链接运行时库

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")    -- 输出目录
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")    -- 中间文件目录

	-- 预编译头
	pchheader "pch.h"
	pchsource "Nazel/src/pch.cpp"

	-- 包含所有h和cpp文件
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/ThirdParty/stb_image/**.h",
		"%{prj.name}/ThirdParty/stb_image/**.cpp",
		"%{prj.name}/ThirdParty/glm/glm/**.hpp",
		"%{prj.name}/ThirdParty/glm/glm/**.inl"
	}

	-- 包含目录
	includedirs
	{
		"%{prj.name}/ThirdParty/spdlog/include",
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	defines{
		"_CRT_SECURE_NO_WARNINGS"
	}

	-- 针对Windows系统
	filter "system:Windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"NZ_PLATFORM_WINDOWS",
			"NZ_BUILD_DLL",
			"GLAD_INCLUDE_NONE"
		}

		-- 复制生成的Nazel.dll文件到Sandbox文件夹
		postbuildcommands
		{
			("{COPYFILE} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "NZ_DEBUG"
		runtime "Debug"
		symbols "On"
	
	filter "configurations:Release"
		defines "NZ_Release"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "NZ_Dist"
		runtime "Release"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	cppdialect "C++17"
	buildoptions "/utf-8"
	language "C++"
	staticruntime "on"	

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
		"Nazel/src",
		"%{IncludeDir.glm}",
		"Nazel/ThirdParty"
	}

	filter "system:windows"
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
		runtime "Debug"
		symbols "On"
	
	filter "configurations:Release"
		defines "NZ_Release"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "NZ_Dist"
		runtime "Release"
		optimize "On"

project "NazelEditor"
	location "NazelEditor"
	kind "ConsoleApp"
	cppdialect "C++17"
	buildoptions "/utf-8"
	language "C++"
	staticruntime "on"	

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
		"Nazel/src",
		"%{IncludeDir.glm}",
		"Nazel/ThirdParty"
	}

	filter "system:windows"
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
		runtime "Debug"
		symbols "On"
	
	filter "configurations:Release"
		defines "NZ_Release"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "NZ_Dist"
		runtime "Release"
		optimize "On"