workspace "AuroraRenderer"
	architecture "x64"
	startproject "Sandbox"
    configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}"

IncludeDir = {}
IncludeDir["ImGui"] = "AuroraRenderer/external/imgui"
IncludeDir["ImGuiBackends"] = "AuroraRenderer/external/imgui/backends"

group "Dependencies"
	include "AuroraRenderer/external/imgui"

group ""

project "AuroraRenderer"
	location "AuroraRenderer"
	kind "StaticLib"
	language "C++"
	cppdialect "C++23"
	staticruntime "on"

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
        "%{IncludeDir.ImGuiBackends}"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	links 
	{ 
		"ImGui"
	}

	filter "configurations:Debug"
		defines "AURORA_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "AURORA_RELEASE"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++23"
	staticruntime "on"

	buildoptions { "/utf-8" }

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")

    files { "%{prj.name}/src/**.h", "%{prj.name}/src/**.cpp" }
	
	includedirs
	{
		"AuroraRenderer/src",
		"AuroraRenderer/external/spdlog/include",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.ImGuiBackends}"
	}

	links 
	{
		"AuroraRenderer",
		"ImGui",
	}
		
	filter "configurations:Debug"
		defines "AURORA_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "AURORA_RELEASE"
		runtime "Release"
		optimize "on"