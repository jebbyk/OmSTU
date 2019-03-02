#include <stdio.h>
#include <windows.h>

int main()
{
    HANDLE hstdin, hstdout;
    DWORD actlen;
    hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
    hstdin = GetStdHandle(STD_INPUT_HANDLE);
    int a = 3,b;
    for(int i = 0; i < 15; i++)
    {
        Sleep(3400);
        printf(GetCommandLine());
        printf(" %d", i);
    }
    return 0;
}