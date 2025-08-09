local localToGlobalDir = "../"
local libraryDir = "../"

project "Veng"
	location (localToGlobalDir .. projectDir)
	kind "StaticLib"

	targetdir (localToGlobalDir .. outputDir)
	objdir (localToGlobalDir .. intermediateDir)

	files
	{
		localToGlobalDir .. projectDir .. "**.h",
		localToGlobalDir .. projectDir .. "**.cpp"
	}

	includedirs
	{
		libraryDir
	}

	externalincludedirs
	{
		localToGlobalDir .. thirdpartyDir .. "glm-1.0.1/include/",
		localToGlobalDir .. thirdpartyDir .. "glad/include/",
		localToGlobalDir .. thirdpartyDir .. "glfw-3.3.8/include/",
		localToGlobalDir .. thirdpartyDir .. "imgui-1.90.3-docking/include/",
		localToGlobalDir .. thirdpartyDir .. "stb/include/"
	}
