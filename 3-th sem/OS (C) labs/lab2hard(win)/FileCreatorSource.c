#include <stdio.h>
#include <conio.h>
#include <Windows.h>

void main()
{
    HANDLE fhandle, hstdin, hstdout;
    DWORD l, length;
    char buffer[128] = "";

    fhandle = CreateFile("output.txt", GENERIC_WRITE, 0,0, CREATE_ALWAYS, 0,0);
    hstdin = GetStdHandle(STD_INPUT_HANDLE);
    hstdout = GetStdHandle(STD_OUTPUT_HANDLE);

    WriteFile(hstdout, "Enter your text\n", 16, &l, NULL);
    ReadFile(hstdin, buffer, 128, &length, NULL);
    WriteFile(fhandle, buffer, length, &l, NULL);
    getchar();

    return;
}