@echo off
call :init
call :checkFolder

if "%isQuit%" equ "1" exit /b
call :prepareFiles
call :compile

if "%isQuit%" equ "1" exit /b
call :link
call :run
exit /b



:init
setlocal enableDelayedExpansion
call :getProjectName
call :setWindowName
set isQuit=0
exit /b

:getProjectName
for %%a in ("%~dp0\.") do set "projectName=%%~nxa"
exit /b

:setWindowName
title %projectName%
exit /b



:checkFolder
for /f %%a in ('dir /b ^| find /v /c ""') do set filesCount=%%a
if %filesCount% equ 1 (call :createTemplate) else (call :checkProject)
exit /b

:createTemplate
call :createSubfolders
call :createSettings
call :createHeader
call :createMain
exit /b

:createSubfolders
mkdir "headers"
mkdir "sources"
mkdir "objects"
exit /b

:createSettings
(
    echo isConsole=1
    echo libraries=0
) > settings.txt
exit /b

:createHeader
(
    echo #ifndef NAME_H_INCLUDED
    echo #define NAME_H_INCLUDED
    echo.
    echo class Name
    echo {
    echo     public:
    echo.
    echo     private:
    echo };
    echo.
    echo #endif // NAME_H_INCLUDED
) > headers\Name.h
exit /b

:createMain
(
    echo #include ^<iostream^>
    echo.
    echo using namespace std;
    echo.
    echo int main(^)
    echo {
    echo     cout ^<^< "test" ^<^< endl;
    echo. 
    echo     cin.get(^);
    echo     return 0;
    echo }
) > sources\main.cpp
exit /b

:checkProject
if exist "settings.txt" if exist "headers" if exist "sources" if exist "objects" exit /b
echo error: not a project folder
pause
set isQuit=1
exit /b



:prepareFiles
for %%a in ("objects\*.o") do call :forObject %%~na
for %%a in ("headers\*.h") do call :forHeader %%~na
exit /b

:forObject
if not exist "sources\%1.cpp" del "objects\%1.o"
exit /b

:forHeader
if not exist "sources\%1.cpp" (
    (
        echo #include "%1.h"
        echo.
    ) > sources\%1.cpp
)
exit /b



:compile
for %%a in ("sources\*.cpp") do call :forSource %%~na
exit /b

:forSource
set fileName=%1
set sourceTime="0"
set headerTime="0"
set objectTime="0"

call :getTime sources\%fileName%.cpp sourceTime
call :getTime headers\%fileName%.h headerTime
call :getTime objects\%fileName%.o objectTime

if "%objectTime%" gtr "%sourceTime%" if "%objectTime%" gtr "%headerTime%" exit /b

echo compiling %fileName%.cpp...
if exist "objects\%fileName%.o" del "objects\%fileName%.o"
g++ -I headers -o objects\%fileName%.o -c sources\%fileName%.cpp
if exist objects\%fileName%.o exit /b

echo error: compilation failed
pause
set isQuit=1
exit /b

:getTime
for %%a in (%1) do set t=%%~ta
for /f "tokens=1-5 delims=.: " %%a in ("%t%") do set %2=%%c%%b%%a%%d%%e
exit /b



:link
if exist "%projectName%.exe" del "%projectName%.exe"
call :getSettings
call :setCommandLineParameters
echo linking...
g++ %parameters%
exit /b

:getSettings
for /f "delims== tokens=1,2" %%a in (settings.txt) do set %%a=%%b
exit /b

:setCommandLineParameters
if "%isConsole%" equ "0" set parameters=%parameters% -mwindows
call :getProjectName
set parameters=%parameters% -Wall -O2 -o %projectName%
set libs= "-L ."
if "%libraries%" neq "0" set parameters=%parameters%%libs%
for %%a in ("objects\*.o") do call :addObject %%~na
if "%libraries%" neq "0" set parameters=%parameters%%libraries%
exit /b

:addObject
set parameters=%parameters% objects\%1.o
exit /b



:run
if exist "%projectName%.exe" ( 
    %projectName%.exe

) else ( 
    echo error: link failed 
    pause 
)
exit /b
