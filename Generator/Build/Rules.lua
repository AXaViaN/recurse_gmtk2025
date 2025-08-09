rootDir = "../../"
binDir = rootDir .. ".bin/"
buildDir = rootDir .. "Build/"
sourceDir = rootDir .. "Source/"
thirdpartyDir = rootDir .. "ThirdParty/"
distributionDir = rootDir .. "Distro/"
generatorDir = rootDir .. "Generator/"

compilerName = _ACTION
outputFolderName = "%{cfg.buildcfg}-%{cfg.system:gsub(\"^%l\", string.upper)}-%{cfg.architecture}"
outputFolder = outputFolderName .. "/%{prj.name}/"
thirdpartyOutputFolder = outputFolderName .. "/ThirdParty/%{prj.name}/"

solutionDir = rootDir
projectDir = sourceDir .. "%{prj.group}/" .. "%{prj.name}/"
outputDir = binDir .. compilerName .. "/" .. outputFolder
intermediateDir = binDir .. "_obj/" .. compilerName .. "/" .. outputFolder

thirdpartySolutionDir = solutionDir .. "ThirdParty/"
thirdpartyProjectDir = thirdpartySolutionDir .. "%{prj.name}/"
thirdpartyOutputDir = binDir .. compilerName .. "/" .. thirdpartyOutputFolder
thirdpartyIntermediateDir = binDir .. "_obj/" .. compilerName .. "/" .. thirdpartyOutputFolder

distributionName = "recurse"
distributionOutputDir = distributionDir .. "Bin/" .. outputFolderName .. "/"

generatorOutputDir = generatorDir .. "%{prj.name}/"

_, depthFromRootDir = rootDir:gsub("/", "")
projectDirDepth = select(2, projectDir:gsub("/", "")) - depthFromRootDir
projectDirToRootDir = string.rep("../", projectDirDepth)

function importProject(projectLabel)
	dofile(sourceDir .. projectLabel .. "/Rules.lua")
end

workspace "recurse"
	location (solutionDir)
	architecture "x64"

	startproject "Game"

	configurations
	{
		"Debug",
		"Release",
		"Shipping"
	}

	language "C++"
	cppdialect "C++latest"
	staticruntime "On"
	systemversion "latest"

	warnings "High"
	externalwarnings "Off"
	externalanglebrackets "On"

	externalincludedirs
	{
	}

	defines
	{
		"AX_PLATFORM_%{cfg.system:upper()}",	-- AX_PLATFORM_WINDOWS etc.
		"AX_CONFIG_%{cfg.buildcfg:upper()}",	-- AX_CONFIG_DEBUG etc.
		"GLM_FORCE_XYZW_ONLY",
		"GLM_FORCE_DEPTH_ZERO_TO_ONE",
		"GLFW_INCLUDE_NONE"
	}

	flags
	{
		"FatalCompileWarnings",
		"MultiProcessorCompile",
		"NoPCH"
	}

	filter "system:windows"
		defines
		{
			"_CRT_SECURE_NO_WARNINGS",
			"_MBCS",
			"_WIN32_WINNT=0x0A00" -- _WIN32_WINNT_WIN10
		}

	filter { "system:windows", "configurations:Debug" }
		disablewarnings
		{
			"4702",
			"4514",
			"26812",
			"4189",
			"4100",
			"4101",
			"4505"
		}

	filter { "system:windows", "configurations:Release or Shipping" }
		fatalwarnings
		{
			"4702"
		}

	filter "configurations:Debug"
		symbols "Full"
		optimize "Off"

	filter "configurations:Release or Shipping"
		optimize "Full"

		flags
		{
			"LinkTimeOptimization"
		}
		
group "Base"
	project "Misc"
		location (projectDir)
		kind "Utility"

		targetdir (outputDir)
		objdir (intermediateDir)

		files
		{
			distributionDir .. "**.**",
			rootDir .. ".**",
			rootDir .. "**.bat",
			rootDir .. "**.lua"
		}

		removefiles
		{
			distributionDir .. "Bin/**"
		}

		vpaths
		{
			["/*"] = { rootDir .. "**.**" }
		}

		filter "system:windows"
			prebuildcommands
			{
				"cd " .. projectDirToRootDir,
				"cd " .. buildDir:gsub(rootDir, ""),
				"call \"" .. compilerName .. ".bat\""
			}

			postbuildcommands
			{
				"cd " .. projectDirToRootDir,
				"rmdir /s /q \"" .. outputDir:gsub(rootDir, "") .. "\"",
				"rmdir /s /q \"" .. intermediateDir:gsub(rootDir, "") .. "\""
			}

	importProject("Base/AX")
	importProject("Base/Veng")

group "Client"
	project "Game"
		location (projectDir)
		kind "ConsoleApp"

		targetdir (distributionOutputDir)
		objdir (intermediateDir)
		targetname (distributionName)

		files
		{
			projectDir .. "**.h",
			projectDir .. "**.cpp"
		}

		includedirs
		{
			projectDir
		}

		externalincludedirs
		{
			sourceDir .. "Base/",
			thirdpartyDir .. "glm-1.0.1/include/",
			thirdpartyDir .. "glad/include/",
			thirdpartyDir .. "glfw-3.3.8/include/",
			thirdpartyDir .. "stb/include/",
			thirdpartyDir .. "miniaudio/include/"
		}

		links
		{
			"Misc",
			"AX",
			"Veng",
			thirdpartyDir .. "glfw-3.3.8/lib-" .. compilerName .. "/glfw3_mt.lib",
			"stb",
			"miniaudio"
		}

		filter { "system:windows" }
			entrypoint "mainCRTStartup"

		filter "configurations:Shipping"
			kind "WindowedApp"

group "ThirdParty"
	project "miniaudio"
		location (thirdpartyProjectDir)
		kind "StaticLib"

		warnings "Off"

		targetdir (thirdpartyOutputDir)
		objdir (thirdpartyIntermediateDir)

		files
		{
			thirdpartyProjectDir .. "**.h",
			thirdpartyProjectDir .. "**.cpp"
		}

		includedirs
		{
			thirdpartyProjectDir .. "include/"
		}

	project "stb"
		location (thirdpartyProjectDir)
		kind "StaticLib"

		warnings "Off"

		targetdir (thirdpartyOutputDir)
		objdir (thirdpartyIntermediateDir)

		files
		{
			thirdpartyProjectDir .. "**.h",
			thirdpartyProjectDir .. "**.cpp"
		}

		includedirs
		{
			thirdpartyProjectDir .. "include/"
		}

		externalincludedirs
		{
			sourceDir .. "Base/"
		}
