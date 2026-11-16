@echo off
setlocal EnableExtensions EnableDelayedExpansion
title RPG - A Lenda de Alvorada 3.2
cd /d "%~dp0"

if not exist "build" mkdir "build"

where gcc >nul 2>nul
if errorlevel 1 (
  if exist "C:\msys64\ucrt64\bin\gcc.exe" (
    set "PATH=C:\msys64\ucrt64\bin;!PATH!"
  ) else if exist "%LOCALAPPDATA%\Programs\MSYS2\ucrt64\bin\gcc.exe" (
    set "PATH=%LOCALAPPDATA%\Programs\MSYS2\ucrt64\bin;!PATH!"
  ) else (
    echo [INFO] GCC nao encontrado. Preparando o ambiente...
    call "%~dp0INSTALAR_AMBIENTE.bat"
    if errorlevel 1 goto ERRO
    if exist "C:\msys64\ucrt64\bin\gcc.exe" set "PATH=C:\msys64\ucrt64\bin;!PATH!"
    if exist "%LOCALAPPDATA%\Programs\MSYS2\ucrt64\bin\gcc.exe" set "PATH=%LOCALAPPDATA%\Programs\MSYS2\ucrt64\bin;!PATH!"
  )
)

where gcc >nul 2>nul
if errorlevel 1 (
  echo [ERRO] GCC continua indisponivel. Execute INSTALAR_AMBIENTE.bat manualmente.
  goto ERRO
)

set "SOURCES="
for %%F in (src\*.c) do set "SOURCES=!SOURCES! %%F"
echo.
echo [1/2] Compilando RPG 3.2...
gcc -std=c11 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -O2 -Iinclude !SOURCES! -o build\rpg.exe
if errorlevel 1 goto ERRO

echo [2/2] Iniciando...
echo.
"build\rpg.exe"
echo.
echo Jogo encerrado.
pause
exit /b 0

:ERRO
echo.
echo [ERRO] A preparacao ou compilacao falhou. Verifique as mensagens acima.
pause
exit /b 1
