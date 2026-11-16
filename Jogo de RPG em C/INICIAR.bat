@echo off
setlocal EnableExtensions
title RPG - A Lenda de Alvorada 3.1
cd /d "%~dp0"

if not exist "build" mkdir "build"

where gcc >nul 2>nul
if errorlevel 1 (
  if exist "C:\msys64\ucrt64\bin\gcc.exe" (
    set "PATH=C:\msys64\ucrt64\bin;%PATH%"
  ) else if exist "%LOCALAPPDATA%\Programs\MSYS2\ucrt64\bin\gcc.exe" (
    set "PATH=%LOCALAPPDATA%\Programs\MSYS2\ucrt64\bin;%PATH%"
  ) else (
    echo [INFO] Ferramentas de compilacao ausentes.
    call "%~dp0INSTALAR_AMBIENTE.bat"
    if errorlevel 1 exit /b 1
    if exist "C:\msys64\ucrt64\bin\gcc.exe" set "PATH=C:\msys64\ucrt64\bin;%PATH%"
    if exist "%LOCALAPPDATA%\Programs\MSYS2\ucrt64\bin\gcc.exe" set "PATH=%LOCALAPPDATA%\Programs\MSYS2\ucrt64\bin;%PATH%"
  )
)

echo.
echo [1/2] Compilando o RPG...
gcc -std=c11 -Wall -Wextra -Wpedantic -O2 -Iinclude src\*.c -o build\rpg.exe
if errorlevel 1 (
  echo.
  echo [ERRO] A compilacao falhou. Leia as mensagens acima.
  pause
  exit /b 1
)

echo [2/2] Iniciando...
echo.
"build\rpg.exe"
echo.
echo O jogo foi encerrado.
pause
endlocal
