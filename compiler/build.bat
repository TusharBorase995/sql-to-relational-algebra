@echo off
setlocal enabledelayedexpansion

cd /d "%~dp0"

echo [1/3] Running Bison...
..\tools\winflexbison\win_bison.exe -d -v -o src\parser.tab.cpp src\parser.y
if %ERRORLEVEL% NEQ 0 (
    echo Bison compilation failed!
    exit /b %ERRORLEVEL%
)

echo [2/3] Running Flex...
..\tools\winflexbison\win_flex.exe -o src\lex.yy.cpp src\lexer.l
if %ERRORLEVEL% NEQ 0 (
    echo Flex compilation failed!
    exit /b %ERRORLEVEL%
)

if not exist "bin" mkdir "bin"

echo [3/3] Compiling C++ binary with g++...
g++ -std=gnu++14 -O2 -Isrc src\parser.tab.cpp src\lex.yy.cpp src\ast.cpp src\schema.cpp src\ra.cpp src\optimizer.cpp src\main.cpp -o bin\sql2ra.exe
if %ERRORLEVEL% NEQ 0 (
    echo C++ compilation failed!
    exit /b %ERRORLEVEL%
)

echo Build succeeded! Binary at compiler\bin\sql2ra.exe
