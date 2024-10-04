@ECHO OFF

SETLOCAL ENABLEDELAYEDEXPANSION

set LIB_DIR=.\libs
if not exist "%LIB_DIR%" (
    echo Creating %LIB_DIR% as it was not found...
    mkdir "%LIB_DIR%"
) else (
    echo Found %LIB_DIR% directory...
)

@REM In case either the GLFW or GLEW links do not work below, please manually extract the necessary libs
@REM from those links and place them into the .\libs folder. You may have to manually build those libraries,
@REM otherwise look for prebuilt binaries. GLFW has prebuilt binaries at https://www.glfw.org/download.html
@REM Alternative links:
@REM GLFW_URL=https://github.com/glfw/glfw/releases/download/3.3.8/glfw-3.3.8.bin.WIN64.zip
@REM GLEW_URL=https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0.zip

set GLFW_URL=https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.bin.WIN64.zip
set GLEW_URL=https://sourceforge.net/projects/glew/files/glew/2.2.0/glew-2.2.0-win32.zip/download

echo Select your build system:
echo 1. MinGW
echo 2. Visual Studio 2013
echo 3. Visual Studio 2015
echo 4. Visual Studio 2017
echo 5. Visual Studio 2019
echo 6. Visual Studio 2022

set /p "BUILD_SYSTEM=Enter the number corresponding to your build system: "

IF NOT "%BUILD_SYSTEM%"=="1" IF NOT "%BUILD_SYSTEM%"=="2" IF NOT "%BUILD_SYSTEM%"=="3" IF NOT "%BUILD_SYSTEM%"=="4" IF NOT "%BUILD_SYSTEM%"=="5" IF NOT "%BUILD_SYSTEM%"=="6" (
    ECHO Invalid selection. Please enter a number between 1 and 6.
    EXIT /B 1
)

set "USE_MT=n"
if %BUILD_SYSTEM% neq 1 (
    set /p "USE_MT=Do you want to use the multithreaded, static version of the run-time library? Only select yes if using Visual Studio with /MT or /MTd configuration (y/n): "
)

echo Downloading GLFW now...
curl -L -o "%LIB_DIR%\glfw.zip" %GLFW_URL%
echo Extracting GLFW now...
7z x "%LIB_DIR%\glfw.zip" -o"%LIB_DIR%\glfw"
del "%LIB_DIR%\glfw.zip"

echo Downloading GLEW now...
curl -L -o "%LIB_DIR%\glew.zip" %GLEW_URL%
echo Extracting GLEW now...
7z x "%LIB_DIR%\glew.zip" -o"%LIB_DIR%\glew"
del "%LIB_DIR%\glew.zip"

echo Done downloading raw dependencies... Proceeding to clean now

move /y "%LIB_DIR%\glew\glew-2.2.0\lib\Release\x64\glew32s.lib" "%LIB_DIR%\glew32s.lib"
rd /s /q "%LIB_DIR%\glew"


set "PATH="
set "FILE="
set "EXT="

if "%USE_MT%"=="y" (
    set "EXT=_mt"
)

if %BUILD_SYSTEM%==1 (
    set "PATH=%LIB_DIR%\glfw\glfw-3.4.bin.WIN64\lib-mingw-w64\libglfw3.a"
    set "FILE=libglfw3.a"
) else (
    set "FILE=glfw3%EXT%.lib"
    if %BUILD_SYSTEM%==2 (
        set "PATH=%LIB_DIR%\glfw\glfw-3.4.bin.WIN64\lib-vc2013\glfw3%EXT%.lib"
    )
    if %BUILD_SYSTEM%==3 (
        set "PATH=%LIB_DIR%\glfw\glfw-3.4.bin.WIN64\lib-vc2015\glfw3%EXT%.lib"
    )
    if %BUILD_SYSTEM%==4 (
        set "PATH=%LIB_DIR%\glfw\glfw-3.4.bin.WIN64\lib-vc2017\glfw3%EXT%.lib"
    )
    if %BUILD_SYSTEM%==5 (
        set "PATH=%LIB_DIR%\glfw\glfw-3.4.bin.WIN64\lib-vc2019\glfw3%EXT%.lib"
    )
    if %BUILD_SYSTEM%==6 (
        set "PATH=%LIB_DIR%\glfw\glfw-3.4.bin.WIN64\lib-vc2022\glfw3%EXT%.lib"
    )
)

echo Found necessary static lib at "%PATH%"
move /y "%PATH%" "%LIB_DIR%\%FILE%"
rd /s /q "%LIB_DIR%\glfw"

ENDLOCAL

echo Done.
PAUSE
