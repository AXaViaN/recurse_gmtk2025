local localToGlobalDir = "../"
local libraryDir = "../"

project "AX"
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
