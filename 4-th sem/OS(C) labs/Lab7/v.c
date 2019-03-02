#include <stdio.h>
#include <windows.h>

int main()
{
    HANDLE hstdin, hstdout;
    DWORD actlen;
    hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
    hstdin = GetStdHandle(STD_INPUT_HANDLE);
    int a = 3,b;
    while(TRUE)
    {
        Sleep(3400);
        printf(GetCommandLine());
    }
    return 0;
}