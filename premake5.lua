workspace "Nazel"	-- sln文件名
	architecture "x64"

	configurations{
		"Debug",
		"Release",
		"Dist"
	}

	-- 启动项目
	startproject "Sandbox"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- -- 包含相对解决方案的目录
-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Nazel/ThirdParty/GLFW/include"
IncludeDir["Glad"] = "Nazel/ThirdParty/Glad/include"
IncludeDir["ImGui"] = "Nazel/ThirdParty/imgui"
IncludeDir["glm"] = "Nazel/ThirdParty/glm"


include "Nazel/ThirdParty/GLFW"
include "Nazel/ThirdParty/Glad"
include "Nazel/ThirdParty/imgui"

project "Nazel"		--Nazel项目
	location "Nazel"	--在sln所属文件夹下的Nazel文件夹
	kind "SharedLib"	--dll动态库
	language "C++"
	buildoptions "/utf-8"
	-- On:代码生成的运行库选项是MTD,静态链接MSVCRT.lib库;
	-- Off:代码生成的运行库选项是MDD,动态链接MSVCRT.dll库;打包后的exe放到另一台电脑上若无这个dll会报错
	staticruntime "Off"	

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")	-- 输出目录
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")	-- 中间目录

	-- 预编译头 
	pchheader "pch.h"
	pchsource "Nazel/src/pch.cpp"

	-- 包含的所有h和cpp文件
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		-- 可以不用包含hpp文件
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	-- 包含目录
	includedirs
	{
		"%{prj.name}/ThirdParty/spdlog/include",
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	-- 如果是window系统
	filter "system:Windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"NZ_PLATFORM_WINDOWS",
			"NZ_BUILD_DLL",
			"GLAD_INCLUDE_NONE"
		}

		-- 编译好后移动Hazel.dll文件到Sandbox文件夹下
		postbuildcommands
		{
			("{COPYFILE} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "NZ_DEBUG"
		runtime "Debug"
		buildoptions "/MDd"
		symbols "On"
	
	filter "configurations:Release"
		defines "NZ_Release"
		runtime "Release"
		--buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "NZ_Dist"
		runtime "Release"
		--buildoptions "/MD"
		optimize "On"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	buildoptions "/utf-8"
	language "C++"
	staticruntime "Off"	

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
		"%{IncludeDir.glm}"
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
		runtime "Debug"
		--buildoptions "/MDd"
		symbols "On"
	
	filter "configurations:Release"
		defines "NZ_Release"
		--buildoptions "/MD"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "NZ_Dist"
		runtime "Release"
		--buildoptions "/MD"
		optimize "On"