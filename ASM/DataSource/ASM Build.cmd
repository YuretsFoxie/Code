@echo off
call :getProjectName
call :clean
call :assemble
pause
call :write
call :clean
pause
exit /b



:getProjectName
for /R %cd% %%f in (*.asm) do set "projectName=%%~nf"
exit /b

:clean
if exist "%projectName%.hex" (del "%projectName%.hex")
exit /b

:assemble
avrasm2.exe -fI -I C:\Data\Tools\ASM\include "%projectName%.asm"
exit /b

:write
avrdude -C "C:\Data\Tools\AVRDude\avrdude.conf" -c ftbb -p m16 -P ft0 -B 1200 -e -U flash:w:"%projectName%.hex":i
exit /b
