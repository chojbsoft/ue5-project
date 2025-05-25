@echo off
echo Start Setup.bat
echo.

echo [pull submodule]
git submodule update --init --recursive
echo.

echo [install vcpkg]
call .\vcpkg\bootstrap-vcpkg.bat
echo.

echo [install boost:x64-windows]
.\vcpkg\vcpkg install boost:x64-windows
echo.

echo [integrate vcpkg]
.\vcpkg\vcpkg integrate install
echo.

echo [done]
pause