ECHO OFF

IF [%1] == [] ECHO "No operation specified!" & EXIT 1

REM install chocolatey
@"%SystemRoot%\System32\WindowsPowerShell\v1.0\powershell.exe" -NoProfile -InputFormat None -ExecutionPolicy Bypass -Command "[System.Net.ServicePointManager]::SecurityProtocol = 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))" && SET "PATH=%PATH%;%ALLUSERSPROFILE%\chocolatey\bin"

REM figure out dependencies
SET "chocodeps=mingw msys2"
REM install dependencies
IF "%chocodeps%" == "" (
	ECHO "No packages specified to install."
) ELSE (
	choco install %chocodeps%
)

SET "SCRIPT_DIR=%~dp0"
SET "REPO=%SCRIPT_DIR%.."
SET "CONFIGURATION_OPTIONS=-G MinGW Makefiles"

cmake -P "%SCRIPT_DIR%cmake\%1.cmake"
