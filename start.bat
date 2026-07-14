@echo off
setlocal
title BobUI
cd /d "%~dp0"

echo [BobUI] Starting...
where go >nul 2>nul
if errorlevel 1 (
    echo [BobUI] go not found. Please install it.
    pause
    exit /b 1
)

go run .

if errorlevel 1 (
    echo [BobUI] Exited with error code %errorlevel%.
    pause
)
endlocal
