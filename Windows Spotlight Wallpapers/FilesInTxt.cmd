@echo off

del files.txt

for /f  "usebackq delims=;" %%A in (`dir /b /A Wallpapers\*.jpg`) do (
echo Wallpapers\%%A>> files.txt
)

::pause