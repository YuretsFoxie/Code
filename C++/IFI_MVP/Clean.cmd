@echo off
cmdow.exe @ /max
cd C:\Data\Code\C++\IFI\
call :deleteObjects
::call :deleteApplication
echo project cleaned
pause
exit /b

:deleteObjects
for %%a in ("objects\*.o") do call :forObject %%~na
exit /b

:deleteApplication
for %%a in ("*.exe") do call :forExe %%~na
exit /b

:forObject
del "objects\%1.o"
echo deleted %1.o
exit /b

:forExe
del "%1.exe"
echo deleted %1.exe
exit /b
