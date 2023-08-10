:: create the library

g++ -c math.cpp
ar rs libmath.a math.o



:: compile the source files

g++ -c class.cpp
g++ -c main.cpp



:: link the files

::-mwindows
g++ -Wall -O2 -o test -L . main.o class.o -lmath
del math.o class.o main.o libmath.a



:: handle the result

@echo off
if exist "test.exe" (
    cls
    test
) else (
    pause
)
