@echo off
setlocal EnableExtensions
title RPG 3.2 - Instalador de dependencias
cd /d "%~dp0"
echo.
echo ============================================================
echo       RPG A LENDA DE ALVORADA - PREPARACAO DO AMBIENTE
echo ============================================================
echo.

where gcc >nul 2>nul
if not errorlevel 1 goto GCC_OK

where winget >nul 2>nul
if errorlevel 1 (
  echo [ERRO] winget nao encontrado.
  echo Instale o App Installer ou instale MSYS2 manualmente: https://www.msys2.org/
  goto ERRO
)

echo [1/3] Instalando MSYS2...
winget install --id MSYS2.MSYS2 -e --silent --accept-package-agreements --accept-source-agreements
set "MSYS_BASH=C:\msys64\usr\bin\bash.exe"
if not exist "%MSYS_BASH%" set "MSYS_BASH=%LOCALAPPDATA%\Programs\MSYS2\usr\bin\bash.exe"
if not exist "%MSYS_BASH%" (
  echo [ERRO] MSYS2 nao foi encontrado apos a instalacao.
  goto ERRO
)

echo [2/3] Atualizando pacotes do MSYS2...
"%MSYS_BASH%" -lc "pacman -Syu --noconfirm"
if errorlevel 1 (
  echo [ERRO] Atualizacao do MSYS2 falhou. Tente executar este instalador novamente.
  goto ERRO
)
"%MSYS_BASH%" -lc "pacman -Syu --noconfirm"
if errorlevel 1 (
  echo [ERRO] Segunda etapa da atualizacao falhou.
  goto ERRO
)

echo [3/3] Instalando GCC UCRT64...
"%MSYS_BASH%" -lc "pacman -S --needed --noconfirm mingw-w64-ucrt-x86_64-gcc"
if errorlevel 1 goto ERRO

:GCC_OK
set "PATH=C:\msys64\ucrt64\bin;%LOCALAPPDATA%\Programs\MSYS2\ucrt64\bin;%PATH%"
where gcc >nul 2>nul
if errorlevel 1 (
  echo [ERRO] GCC nao esta no PATH. Verifique a instalacao do MSYS2.
  goto ERRO
)
gcc --version

where code >nul 2>nul
if not errorlevel 1 (
  echo [INFO] Instalando extensao C/C++ no VS Code (opcional)...
  code --install-extension ms-vscode.cpptools --force
) else (
  echo [INFO] VS Code nao encontrado. Ele e opcional.
)

echo.
echo [OK] Dependencias preparadas.
echo Execute INICIAR.bat para compilar e jogar.
pause
exit /b 0

:ERRO
echo.
echo [ERRO] Nao foi possivel preparar todas as dependencias.
pause
exit /b 1
