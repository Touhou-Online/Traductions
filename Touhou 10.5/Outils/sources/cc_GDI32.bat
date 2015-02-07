@prompt -----^> 
gcc -c code.c -Wall -Wextra
nasm gdi32.asm -f Win32
ld -o i32.dll -shared gdi32.obj gdi32.def code.o -LE:\Program_Files\MinGW\lib -lkernel32 -lgdi32 -lmsvcrt -e _DllMain@12
@del gdi32.obj code.o 2>nul
@prompt ^> 
