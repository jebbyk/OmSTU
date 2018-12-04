#include "stdio.h"
#include <windows.h>
//#include <dos.h>

void main() {
    char buffer[128];
    int error = 0, bites = 0;
    HANDLE hstdout, fileHandle;
    DWORD actlength, length;
    COORD cPos;
    cPos.X = 60;
    cPos.Y = 15;

    hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
    while(1)
    {
        system("CLS");
        WriteFile(hstdout, "Press enter to open file\n", 25, &actlength, NULL);
        getchar();
        fileHandle = CreateFile("input.txt", GENERIC_WRITE | GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
        if(fileHandle == -1)
        {
            error = GetLastError();
            if(error == ERROR_SHARING_VIOLATION)
            {
                WriteFile(hstdout, "can't open file! it's being used by another process\n", 52, &length, NULL);
                Sleep(3000);
            }
            if(error == ERROR_FILE_NOT_FOUND)
            {
                WriteFile(hstdout, "The system cannot find the file\n", 32, &length, NULL);
                WriteFile(hstdout, "Press any key to exit\n", 22, &length, NULL);
                getchar();
                Sleep(3000);
                return;
            }
        }else{
            WriteFile(hstdout,"File opened\n", 12, &actlength, NULL);
            ReadFile(fileHandle, buffer, 80, &length, 0);
            SetConsoleCursorPosition(hstdout, cPos);
            SetConsoleTextAttribute(hstdout, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            WriteFile(hstdout, buffer, length, &actlength, NULL);
            SetConsoleTextAttribute(hstdout, FOREGROUND_INTENSITY);
            getchar();
            Sleep(7000);
            CloseHandle(fileHandle);
            return;
        }
    }
}