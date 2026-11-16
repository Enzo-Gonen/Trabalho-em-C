@echo off
setlocal EnableExtensions
title RPG - Preparacao do ambiente
cd /d "%~dp0"

echo.
echo ============================================================
echo          RPG A LENDA DE ALVORADA - INSTALADOR
echo ============================================================
echo.

where gcc >nul 2>nul
if not errorlevel 1 goto GCC_OK

echo [INFO] GCC nao encontrado. Verificando o gerenciador winget...
where winget >nul 2>nul
if errorlevel 1 (
  echo [ERRO] O winget nao esta disponivel.
  echo Instale o App Installer pela Microsoft Store e execute novamente.
  echo Alternativamente, instale MSYS2 em https://www.msys2.org/
  pause
  exit /b 1
)

echo [INFO] Instalando MSYS2...
winget install --id MSYS2.MSYS2 -e --silent --accept-package-agreements --accept-source-agreements
if errorlevel 1 (
  echo [AVISO] O instalador pode ja existir ou exigir permissao.
)

set "MSYS_BASH=C:\msys64\usr\bin\bash.exe"
if not exist "%MSYS_BASH%" set "MSYS_BASH=%LOCALAPPDATA%\Programs\MSYS2\usr\bin\bash.exe"
if not exist "%MSYS_BASH%" (
  echo [ERRO] Nao encontrei o MSYS2 em seu local padrao.
  echo Abra o MSYS2 uma vez e execute novamente este instalador.
  pause
  exit /b 1
)

echo [INFO] Atualizando os pacotes do MSYS2...
"%MSYS_BASH%" -lc "pacman -Syu --noconfirm"
"%MSYS_BASH%" -lc "pacman -Syu --noconfirm"
echo [INFO] Instalando GCC para Windows (UCRT64)...
"%MSYS_BASH%" -lc "pacman -S --needed --noconfirm mingw-w64-ucrt-x86_64-gcc"
if errorlevel 1 (
  echo [ERRO] Nao foi possivel instalar o GCC.
  echo Execute novamente INSTALAR_AMBIENTE.bat para concluir atualizacoes pendentes.
  pause
  exit /b 1
)

set "PATH=C:\msys64\ucrt64\bin;%PATH%"
if exist "%LOCALAPPDATA%\Programs\MSYS2\ucrt64\bin" set "PATH=%LOCALAPPDATA%\Programs\MSYS2\ucrt64\bin;%PATH%"
where gcc >nul 2>nul
if errorlevel 1 (
  echo [ERRO] GCC foi instalado, mas nao foi localizado no PATH desta sessao.
  pause
  exit /b 1
)

:GCC_OK
echo [OK] Compilador encontrado:
gcc --version

where code >nul 2>nul
if not errorlevel 1 (
  echo [INFO] Instalando extensao C/C++ do VS Code...
  code --install-extension ms-vscode.cpptools --force
) else (
  echo [INFO] VS Code nao encontrado. O jogo funciona sem VS Code.
)

echo.
echo [OK] Ambiente preparado.
echo Execute INICIAR.bat para compilar e abrir o RPG.
pause
endlocal
