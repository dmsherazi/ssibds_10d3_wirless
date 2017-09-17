rem CMD ,run "SIM800H32.bat all" or "SIM800H32.bat clean"
@echo off
set path=%CD%\build\winmake;%path%
rem make -f build\Makefile --debug=b PROJ=%~n0 CFG=user %1
make -f build\Makefile PROJ=%~n0 CFG=user %1
