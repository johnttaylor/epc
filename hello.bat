@echo off
echo:buildnum: %1, %2. Hello I am bob
cat README.md
if NOT "/%3"=="/" exit /b 1
exit /b 0
