@echo off
REM Build script for creating Windows executable from Python game
REM This creates a standalone .exe file for Windows

echo ==========================================
echo   Building Dungeon Crawler Executable
echo ==========================================
echo.

REM Check if Python is installed
python --version >nul 2>&1
if errorlevel 1 (
    echo Error: Python is not installed
    echo Please install Python 3.6 or higher from python.org
    exit /b 1
)

echo Python found
echo.

REM Check if PyInstaller is installed
python -m pip show pyinstaller >nul 2>&1
if errorlevel 1 (
    echo PyInstaller not found. Installing...
    python -m pip install pyinstaller
    if errorlevel 1 (
        echo Failed to install PyInstaller
        exit /b 1
    )
    echo PyInstaller installed
) else (
    echo PyInstaller found
)

echo.
echo Building executable...
echo.

REM Build the executable
python -m PyInstaller --onefile --name "DungeonCrawler" game.py

if errorlevel 0 (
    echo.
    echo ==========================================
    echo   Build Successful!
    echo ==========================================
    echo.
    
    if exist "dist\DungeonCrawler.exe" (
        echo Executable location: dist\DungeonCrawler.exe
        dir dist\DungeonCrawler.exe
        echo.
        echo To run the game:
        echo   dist\DungeonCrawler.exe
        echo.
        echo To distribute, copy DungeonCrawler.exe from the dist\ directory
    )
) else (
    echo.
    echo Build failed!
    exit /b 1
)

echo.
pause
