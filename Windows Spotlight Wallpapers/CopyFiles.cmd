@echo off
title Windows Spotlight Reflection

echo deleting old files

del Wallpapers\*.jpg

echo Coping files... 

copy %localappdata%\Packages\Microsoft.Windows.ContentDeliveryManager_cw5n1h2txyewy\LocalState\Assets Wallpapers\

ren Wallpapers\* *.jpg

echo Task Completed
::pause