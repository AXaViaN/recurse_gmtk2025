@echo off

cd ..

goto main

:remove_folder
	if exist %~1 (
		@echo Removing %1
		rmdir /s /q %~1
	)
exit /b 0

:main
	call :remove_folder .bin
	call :remove_folder .vs
	call :remove_folder Distro\Bin

	del /s /q *.sln
	del /s /q *.vcxproj
	del /s /q *.vcxproj.filters
	del /s /q *.vcxproj.user

	del /s /q *.log
	del /s /q *.pdb
	del /s /q *.ini

	@echo.
	@echo Build system cleanup finished.
	@echo.
	pause
