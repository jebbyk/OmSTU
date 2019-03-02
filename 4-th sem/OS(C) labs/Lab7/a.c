
#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <conio.h>
int main()
{
    HANDLE hstdin, hstdout, hJob1, hJob2;
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



    hJob1 = CreateJobObject(NULL, NULL);
    hJob2 = CreateJobObject(NULL, NULL);

    if (CreateProcess("c.exe"," My name Child Volodya 0",NULL,NULL,TRUE,NULL,NULL,NULL,&cif,&pi)==TRUE)
    {
        AssignProcessToJobObject(hJob1,pi.hProcess);
        WriteFile(hstdout, text2,sizeof(text2),&actlen, NULL);       
    }else{
        WriteFile(hstdout, text3,sizeof(text3),&actlen, NULL);
    }

    if (CreateProcess("c.exe"," My name child Pasha 0",NULL,NULL,TRUE,NULL,NULL,NULL,&cif,&pi)==TRUE)
    {
        AssignProcessToJobObject(hJob1,pi.hProcess);
        WriteFile(hstdout, text2,sizeof(text2),&actlen, NULL);       
    }else{
        WriteFile(hstdout, text3,sizeof(text3),&actlen, NULL);
    }
    
   for(int i = 0; i < 11; i++)
   {
       Sleep(2000);
       if(i == 7) TerminateJobObject(hJob1,0);
   }
    TerminateJobObject(hJob2,0);
    return 0;
}