workspace "AuroraRenderer"
	architecture "x64"
	startproject "Sandbox"
    configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}"

directxtk_version = "2025.7.10.1"
nuget_packages = "packages"


IncludeDir = {}
IncludeDir["ImGui"] = "AuroraRenderer/external/imgui"
IncludeDir["ImGuiBackends"] = "AuroraRenderer/external/imgui/backends"
IncludeDir["DirectXTK"] = nuget_packages .. "/directxtk_desktop_win10." .. directxtk_version .. "/include"

LibDir = {}
LibDir["DirectXTK"] = nuget_packages .. "/directxtk_desktop_win10." .. directxtk_version .. "/native/lib"

group "Dependencies"
	include "AuroraRenderer/external/imgui"

group ""

project "AuroraRenderer"
	location "AuroraRenderer"
	kind "StaticLib"
	language "C++"
	cppdialect "C++23"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")

	buildoptions { "/utf-8" }

	pchheader "aurorapch.h"
	pchsource "AuroraRenderer/src/aurorapch.cpp"

    files { "%{prj.name}/src/**.h", "%{prj.name}/src/**.cpp" }
	
	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/external/spdlog/include",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.ImGuiBackends}",
        "%{IncludeDir.DirectXTK}"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	links 
	{ 
		"ImGui",
		"DirectXTK"
	}

	filter "configurations:Debug"
		defines "AURORA_DEBUG"
		runtime "Debug"
		symbols "on"
		libdirs { "%{LibDir.DirectXTK}/x64/Debug" }

	filter "configurations:Release"
		defines "AURORA_RELEASE"
		runtime "Release"
		optimize "on"
		libdirs { "%{LibDir.DirectXTK}/x64/Release" }

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++23"
	staticruntime "off"
	
	buildoptions { "/utf-8" }

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")

    files { "%{prj.name}/src/**.h", "%{prj.name}/src/**.cpp" }
	
	includedirs
	{
		"AuroraRenderer/src",
		"AuroraRenderer/external/spdlog/include",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.ImGuiBackends}",
        "%{IncludeDir.DirectXTK}"
	}

	links 
	{
		"AuroraRenderer",
		"ImGui",
		"DirectXTK"
	}
        
    filter "configurations:Debug"
        defines "AURORA_DEBUG"
        runtime "Debug"
        symbols "on"
		libdirs { "%{LibDir.DirectXTK}/x64/Debug" }

    filter "configurations:Release"
        defines "AURORA_RELEASE"
        runtime "Release"
        optimize "on"
		libdirs { "%{LibDir.DirectXTK}/x64/Release" }