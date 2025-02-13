workspace "Nazel"	-- sln�ļ���
	architecture "x64"

	configurations{
		"Debug",
		"Release",
		"Dist"
	}

	-- ������Ŀ
	startproject "Sandbox"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- -- ������Խ��������Ŀ¼
-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Nazel/ThirdParty/GLFW/include"
IncludeDir["Glad"] = "Nazel/ThirdParty/Glad/include"
IncludeDir["ImGui"] = "Nazel/ThirdParty/imgui"
IncludeDir["glm"] = "Nazel/ThirdParty/glm"


include "Nazel/ThirdParty/GLFW"
include "Nazel/ThirdParty/Glad"
include "Nazel/ThirdParty/imgui"

project "Nazel"		--Nazel��Ŀ
	location "Nazel"	--��sln�����ļ����µ�Nazel�ļ���
	kind "SharedLib"	--dll��̬��
	language "C++"
	buildoptions "/utf-8"
	-- On:�������ɵ����п�ѡ����MTD,��̬����MSVCRT.lib��;
	-- Off:�������ɵ����п�ѡ����MDD,��̬����MSVCRT.dll��;������exe�ŵ���һ̨�������������dll�ᱨ��
	staticruntime "Off"	

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")	-- ���Ŀ¼
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")	-- �м�Ŀ¼

	-- Ԥ����ͷ 
	pchheader "pch.h"
	pchsource "Nazel/src/pch.cpp"

	-- ����������h��cpp�ļ�
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		-- ���Բ��ð���hpp�ļ�
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	-- ����Ŀ¼
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

	-- �����windowϵͳ
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

		-- ����ú��ƶ�Hazel.dll�ļ���Sandbox�ļ�����
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