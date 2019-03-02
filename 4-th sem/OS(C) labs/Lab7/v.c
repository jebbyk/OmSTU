#include <stdio.h>
#include <windows.h>

int main()
{
    HANDLE hstdin, hstdout;
    DWORD actlen;
    hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
    hstdin = GetStdHandle(STD_INPUT_HANDLE);
    int a = 3,b;
    char text1[] = "\n Hello!! I'm a vnuk process";
    while(TRUE)
    {
        Sleep(1700);
        printf(GetCommandLine());
    }
    return 0;
}