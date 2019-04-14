#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void main()
{
    char *pb, st[20];
    HANDLE hmem;
    int k;
    HANDLE hmtx;
    hmtx = OpenMutex(SYNCHRONIZE, TRUE,"myMutex");
    

   if(hmtx != NULL )
   {
        hmem=OpenFileMapping(FILE_MAP_READ, FALSE, "SHAREMEM_MYY");
        WaitForSingleObject(hmtx, INFINITE);
        if (hmem==0)
        {
            printf("Error OpenSharedMem with RC=%ld\n", GetLastError());
            getchar(); 
            exit(0);
        }
        pb=MapViewOfFile(hmem, FILE_MAP_READ,0,0,0);
        if (pb==NULL)
        {
            printf("Error Mapping SharedMem with RC=%ld\n", GetLastError());
            getchar(); 
            exit(0);
        }
        for (k=0;k<10;k++ )
        {
            strncpy(st, pb, 20); 
            st[19]='\0'; 
            printf("%s\n", st); 
            Sleep(2000);
        }
        UnmapViewOfFile(pb); CloseHandle(hmem);
        ReleaseMutex(hmtx);
   }
}