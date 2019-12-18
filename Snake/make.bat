@echo off
echo Building a release version
echo [1/10] Compiling ./Rank.c
gcc -c -Dwindows -O2 ./Rank.c -o ./Rank.o
echo [2/10] Compiling ./Block.c
gcc -c -Dwindows -O2 ./Block.c -o ./Block.o
echo [3/10] Compiling ./ConsoleIO.c
gcc -c -Dwindows -O2 ./ConsoleIO.c -o ./ConsoleIO.o
echo [4/10] Compiling ./Config.c
gcc -c -Dwindows -O2 ./Config.c -o ./Config.o
echo [5/10] Compiling ./main.c
gcc -c -Dwindows -O2 ./main.c -o ./main.o
echo [6/10] Compiling ./InputBox.c
gcc -c -Dwindows -O2 ./InputBox.c -o ./InputBox.o
echo [7/10] Compiling ./Menu.c
gcc -c -Dwindows -O2 ./Menu.c -o ./Menu.o
echo [8/10] Compiling ./Snake.c
gcc -c -Dwindows -O2 ./Snake.c -o ./Snake.o
echo [9/10] Compiling ./List.c
gcc -c -Dwindows -O2 ./List.c -o ./List.o
echo [10/10] Building snake
gcc -Dwindows -O2 ./Rank.o ./Block.o ./ConsoleIO.o ./Config.o ./main.o ./InputBox.o ./Menu.o ./Snake.o ./List.o -o snake
