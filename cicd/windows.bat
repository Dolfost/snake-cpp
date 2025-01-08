ECHO OFF

IF [%1] == [] ECHO "No operation specified!" & EXIT 1

REM install chocolatey
@"%SystemRoot%\System32\WindowsPowerShell\v1.0\powershell.exe" -NoProfile -InputFormat None -ExecutionPolicy Bypass -Command "[System.Net.ServicePointManager]::SecurityProtocol = 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))" && SET "PATH=%PATH%;%ALLUSERSPROFILE%\chocolatey\bin"

REM figure out dependencies
SET "chocodeps=msys2"
REM install dependencies
IF "%chocodeps%" == "" (
	ECHO "No packages specified to install."
) ELSE (
	choco install %chocodeps%
)

C:\\msys64\\usr\\bin\\env MSYSTEM=MSYS /usr/bin/bash -l -c "pacman -S mingw-w64-x86_64-ncurses mingw-w64-ucrt-x86_64-gcc cmake git"

SET "SCRIPT_DIR=%~dp0"
SET "REPO=%SCRIPT_DIR%.."
SET "CONFIGURATION_OPTIONS=-G MinGW Makefiles"

cmake -P "%SCRIPT_DIR%cmake\%1.cmake"
