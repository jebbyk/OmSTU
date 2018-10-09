#include <Windows.h>
#include <stdio.h>
#include <conio.h>


void main()
{
    HANDLE fhandle1, fhandle2, fhandle3, hstdin, hstdout;
    DWORD length, l;
    char buffer[128] = "";
    char number[10] = "";

    hstdin = GetStdHandle(STD_INPUT_HANDLE);
    hstdout = GetStdHandle(STD_OUTPUT_HANDLE);

    fhandle1 = CreateFile("output.txt", GENRIC_READ, 0,0, OPEN_ALWAYS, 0, 0);
    DuplicateHandle(GetCurrentProcess(), fhandle1, GetCurrentProcess(), &fhandle2, 0, FALSE, DUPLICATE_SAME_ACCESS);
    fhandle3 = CreateFile("output.txt", GENERIC_READ, 0, 0, OPEN_ALWAYS, 0, 0);

    SetFilePointer(fhandle1, 10, NULL, FILE_BEGIN);

    sprintf(number, "%d\n", fhandle1);
    WriteFile(hstdout, number, 10, &l, NULL);
    sprintf(number, "%d\n", fhandle2);
    WriteFile(hstdout, number, 10, &l, NULL);
    sprintf(number,, "%d\n", fhandle3);
    WriteFile(hstdout, number, 10, &l, NULL);

    WriteFile(hstdout, "\nText from first handle\n", 24, &l, NULL);
    ReadFile(flhandle1, buffer, 7, &l, NULL);
    WriteFile(hstdout, buffer, 7, &l, NULL);

    WriteFile(hstdout, "\nText from second handle\n", 26, &l, NULL);
    ReadFile(flhandle2, buffer, 7, &l, NULL);
    WriteFile(hstdout, buffer, 7, &l, NULL);

    WriteFile(hstdout, "\nText from third handle\n", 24, &l, NULL);
    ReadFile(flhandle3, buffer, 7, &l, NULL);
    WriteFile(hstdout, buffer, 7, &l, NULL);

    getchar();

    return;



}