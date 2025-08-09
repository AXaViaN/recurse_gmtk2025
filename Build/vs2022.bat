@echo off

set compiler=%~n0

cd ..

goto main

:main
	set generator=Generator\Build\Generate_VisualStudio.bat

	call %generator% %compiler%
