#include <stdio.h>
#include <windows.h>

void main()
{
    HANDLE hstdin, hstdout;
    DWORD actlen;
    hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
    hstdin = GetStdHandle(STD_INPUT_HANDLE);
    int a = 3,b;
    char text1[] = "\n Hello!! I'm a vnuk yopta process";
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 1000000000; j++)
        {
            b = i*a;
        }
        printf("\n %d, %d", i, b);
    }
    return 0;
}