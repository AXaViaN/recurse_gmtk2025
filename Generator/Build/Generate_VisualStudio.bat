@echo off

set compiler=%~1

if "%compiler%" == "" (
	@echo Do not run this bat file directly. Use a suitable bat file for your platform from the "Build/" folder at the root.
	@echo.
	pause
	exit /b 0
)

goto main

:main
	set premake=ThirdParty\premake-5.0.0-beta2\bin\premake5.exe
	set rulefile=Generator\Build\Rules.lua

	call %premake% %compiler% --file=%rulefile%

	@echo.
	@echo Build file generation finished.
	@echo.
	pause
