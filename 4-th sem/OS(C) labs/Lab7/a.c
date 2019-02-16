
#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <conio.h>

void main()
{
    HANDLE hstdin, hstdout;
    DWORD actlen;
    hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
    hstdin = GetStdHandle(STD_INPUT_HANDLE);
    char text1[] = "\n Hello!! I'm the main process";
    char text2[] = "\n Creating child process";
    char text3[] = "\n Error creating process";
    WriteFile(hstdout, text1, sizeof(text1), &actlen, NULL);
    
    STARTUPINFO cif;
	ZeroMemory(&cif,sizeof(STARTUPINFO));
	PROCESS_INFORMATION pi;
    for(int i = 0; i < 8; i++)
    {
        if (CreateProcess("c.exe",NULL,NULL,NULL,FALSE,NULL,NULL,NULL,&cif,&pi)==TRUE)
        {
            WriteFile(hstdout, text2,sizeof(text2),&actlen, NULL);
            
        }else{
            WriteFile(hstdout, text3,sizeof(text3),&actlen, NULL);
        }
    }
    return 0;
}