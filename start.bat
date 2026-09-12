@echo off
title SQL to Relational Algebra Compiler Launcher
cd /d "%~dp0"

echo ==========================================================
echo   SQL-to-Relational-Algebra Compiler Suite
echo ==========================================================
echo.

if not exist "compiler\bin\sql2ra.exe" (
    echo [1/3] Building compiler binary...
    call compiler\build.bat
    if %ERRORLEVEL% NEQ 0 (
        echo Compilation failed! Exiting.
        pause
        exit /b 1
    )
) else (
    echo [1/3] Compiler binary found at compiler\bin\sql2ra.exe
)

echo [2/3] Starting Backend Server (port 5000)...
start "Backend (Express)" cmd /k "cd backend && npm start"

timeout /t 2 /nobreak >nul

echo [3/3] Starting Frontend Web App (port 5173)...
start "Frontend (React + Vite)" cmd /k "cd frontend && npm run dev"

timeout /t 3 /nobreak >nul

echo Opening browser at http://localhost:5173...
start http://localhost:5173

echo.
echo ==========================================================
echo   Application successfully started!
echo   Frontend: http://localhost:5173
echo   Backend:  http://localhost:5000
echo ==========================================================
