#include <stdio.h>
#include <windows.h>
#include <conio.h>
int main(int argc, char *argv[])
{

    HANDLE hstdin, hstdout;
    DWORD actlen;
    hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
    hstdin = GetStdHandle(STD_INPUT_HANDLE);
    int a = 3,b;
    char text1[] = "\n Hello!! I'm a child process";
    char text2[] = "\n Creating vnuk yopta process";
    char text3[] = "\n Error creating yopta process";
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 1000000000; j++)
        {
            b = i*a;
        }
        printf("\n %d, %d", i, b);
    }


    STARTUPINFO cif;
	ZeroMemory(&cif,sizeof(STARTUPINFO));
	PROCESS_INFORMATION pi;
        if (CreateProcess("v.exe",NULL,NULL,NULL,FALSE,NULL,NULL,NULL,&cif,&pi)==TRUE)
        {
            WriteFile(hstdout, text2,sizeof(text2),&actlen, NULL);
            
        }else{
            WriteFile(hstdout, text3,sizeof(text3),&actlen, NULL);
        }

    return 0;
}