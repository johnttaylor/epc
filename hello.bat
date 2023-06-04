@echo off
echo:Hello I am bob
cat README.md
if NOT "/%1"=="/" exit /b 1
exit /b 0
