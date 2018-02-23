@echo off

:: Create a index file with the names of the photos in the working folder
for /f  "usebackq delims=;" %%A in (`dir /b /A %USERPROFILE%\Pictures\Windows_Wallpapers\*.jpg`) do (
echo %USERPROFILE%\Pictures\Windows_Wallpapers\%%A >> %USERPROFILE%\Pictures\Windows_Wallpapers\files.txt
)

::pause