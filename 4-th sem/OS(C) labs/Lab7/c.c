#include <stdio.h>
#include <windows.h>
#include <conio.h>


int main()
{

    HANDLE hstdin, hstdout, hJob;
    DWORD actlen;
    hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
    hstdin = GetStdHandle(STD_INPUT_HANDLE);
    int a = 3,b;
    char text2[] = "\n Creating vnuk process";
    char text3[] = "\n Error creating vnuk process";


    STARTUPINFO cif;
	ZeroMemory(&cif,sizeof(STARTUPINFO));
	PROCESS_INFORMATION pi;

    char name[] = "\nVnuk Valera of ";
    strncat(name, GetCommandLine(), 16);

        if (CreateProcess("v.exe",name,NULL,NULL,TRUE,CREATE_NEW_CONSOLE,NULL,NULL,&cif,&pi)==TRUE)
        {
            WriteFile(hstdout, text2,sizeof(text2),&actlen, NULL);
        }else{
            WriteFile(hstdout, text3,sizeof(text3),&actlen, NULL);
        }

       for(int i = 0; i < 20; i++)
        {            
            Sleep(2600);
            printf("\n");
            printf(GetCommandLine());
            printf(" %d", i);
        }

    return 0;
}