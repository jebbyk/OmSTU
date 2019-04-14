#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void main()
{
    char *pm;
    HANDLE hmem, hmtx;
    hmem=CreateFileMapping((HANDLE)-1,NULL,  PAGE_READWRITE, 0, 16000, "SHAREMEM_MYY");
    hmtx=CreateMutex(NULL, FALSE,"myMutex");
    WaitForSingleObject(hmtx, INFINITE);
    if (hmem==0)
    {
        printf("Error AllocSharedMem with RC=%ld\n", GetLastError());
        getchar(); 
        exit(0); 
    }
    pm=MapViewOfFile(hmem, FILE_MAP_WRITE,0,0,0);
    if (pm==NULL)
    {
        printf("Error Mapping SharedMem with RC=%ld\n", GetLastError());
        getchar(); 
        exit(0);
    }
    STARTUPINFO cif;
	ZeroMemory(&cif,sizeof(STARTUPINFO));
	PROCESS_INFORMATION pi;
    ReleaseMutex(hmtx);
    CreateProcess("b.exe","Child of main",NULL,NULL,TRUE,CREATE_NEW_CONSOLE,NULL,NULL,&cif,&pi);
    Sleep(4000);
    WaitForSingleObject(hmtx, INFINITE);
    strcpy(pm, "Privet Shara !!!"); 
    ReleaseMutex(hmtx);
   
    printf("Middle work\n"); 
    Sleep(10000);
    printf("Two step\n");
    strcpy(pm+7, "- Good Bye !!!"); 
    Sleep(10000);
    UnmapViewOfFile(pm);
    CloseHandle(hmem);
} 
