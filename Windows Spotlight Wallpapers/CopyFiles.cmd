@echo off
title Windows Spotlight Wallpapers

:: Move previus photos into the Older folder
move /Y %USERPROFILE%\Pictures\Windows_Wallpapers\*jpg %USERPROFILE%\Pictures\Windows_Wallpapers\Older\

:: Delete any useless remainig file
del /Q %USERPROFILE%\Pictures\Windows_Wallpapers\*

echo Coping files... 

:: Copy downoaded photos to our working folder
copy %localappdata%\Packages\Microsoft.Windows.ContentDeliveryManager_cw5n1h2txyewy\LocalState\Assets %USERPROFILE%\Pictures\Windows_Wallpapers

:: Rename all files to -.jpg
ren %USERPROFILE%\Pictures\Windows_Wallpapers\* *.jpg

::pause