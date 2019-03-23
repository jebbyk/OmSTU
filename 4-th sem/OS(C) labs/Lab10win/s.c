#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void main()
{
    char *pm;
    HANDLE hmem;
    hmem=CreateFileMapping((HANDLE)0xFFFFFFFF,NULL,  PAGE_READWRITE, 0, 16000, "SHAREMEM_MYY");
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
    Sleep(4000);
    strcpy(pm, "Privet Shara !!!"); 
    printf("Middle work\n"); 
    Sleep(10000);
    printf("Two step\n");
    strcpy(pm+7, "- Good Bye !!!"); 
    Sleep(10000);
    UnmapViewOfFile(pm);
    CloseHandle(hmem);
} 
