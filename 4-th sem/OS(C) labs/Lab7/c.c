#include <stdio.h>
#include <windows.h>
#include <conio.h>
int main(char *argv[])
{

    HANDLE hstdin, hstdout, hJob;
    DWORD actlen;
    hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
    hstdin = GetStdHandle(STD_INPUT_HANDLE);
    int a = 3,b;
    char text1[] = "\n Hello!! I'm a child process";
    char text2[] = "\n Creating vnuk process";
    char text3[] = "\n Error creating vnuk process";


    STARTUPINFO cif;
	ZeroMemory(&cif,sizeof(STARTUPINFO));
	PROCESS_INFORMATION pi;

        if (CreateProcess("v.exe","\nMy name vnuchok Valera",NULL,NULL,TRUE,NULL,NULL,NULL,&cif,&pi)==TRUE)
        {
           // AssignProcessToJobObject(hJob,pi.hProcess);
            WriteFile(hstdout, text2,sizeof(text2),&actlen, NULL);
        }else{
            WriteFile(hstdout, text3,sizeof(text3),&actlen, NULL);
        }

        while(TRUE)
        {
            Sleep(2600);
            printf(GetCommandLine());
        }

    return 0;
}