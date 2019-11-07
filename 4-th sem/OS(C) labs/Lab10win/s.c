#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void main()
{
    char *pm;
    HANDLE hmem, hmtx, hstdout;
    hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
    
   /* hmem=CreateFileMapping((HANDLE)-1,NULL,  PAGE_READWRITE, 0, 16000, "SHAREMEM_MYY");
    hmtx=CreateMutex(NULL, FALSE,"myMutex");
    if (hmem==0)
    {
        printf("Error AllocSharedMem with RC=%ld\n", GetLastError());
        getchar(); 
        exit(0); 
    }
    pm=MapViewOfFile(hmem, FILE_MAP_WRITE,0,0,0);
    printf("Mem addr %p\n", pm);
    if (pm==NULL)
    {
        printf("Error Mapping SharedMem with RC=%ld\n", GetLastError());
        getchar(); 
        exit(0);
    }
    STARTUPINFO cif;
	ZeroMemory(&cif,sizeof(STARTUPINFO));
	PROCESS_INFORMATION pi;
    CreateProcess("b.exe","Child of main",NULL,NULL,TRUE,CREATE_NEW_CONSOLE,NULL,NULL,&cif,&pi);
    Sleep(8000);
    WaitForSingleObject(hmtx, INFINITE);
    printf("writing to pm\n");
    strcpy(pm, "Privet Shmara !!!"); 
    ReleaseMutex(hmtx);
    printf("writing complited\n"); 
    //Sleep(10000);
    UnmapViewOfFile(pm);*/
   
    char *pm2;
    
    pm2 = VirtualAlloc(NULL, 5000, MEM_COMMIT, PAGE_READWRITE );
    printf("addr: %p\n", pm2);
    *pm2 = 'e';
    printf("addr: %p, cont: %c\n", pm2, *pm2);
    *(pm2+400) = 'n';
    printf("addr: %p, cont: %c\n", pm2+400, *(pm2+400));
    if(pm2 == NULL)
    {
        printf("Error alocating mem %d", GetLastError());
        exit(0);
    }
    //QueryMem(pm2, 1000);
    char lbuk[] = "abcdefghijklmnopqrst";
    *(pm2+400) = lbuk[0];
    printf("addr: %p, cont: %c\n", pm2+400, *(pm2+400));
    printf("\n\n\n\n\n");
    for(int i = 0;  i < 15; i++)
    {
       // *(pm2+i*400 - 1) = lbuk[i];
        //printf("writing: %c\n",lbuk[i]/*pm2 + i*400 -1*/);
        //printf("hy\n");
        *(pm2+400*i) = lbuk[i];
        printf("addr: %p, cont: %c\n", pm2+400*i , *(pm2+400*i));
    }
   
    //VirtualFree(pm2, 10000, MEM_RELEASE);
    printf("addr: %p, cnt: %c\n", pm2, *pm2);
    printf("addr: %p, cnt: %c\n", pm2+400, *(pm2+400));
    printf("\n\n\n\n");
    for(int i = 0;  i < 15; i++)
    {
       // *(pm2+i*400 - 1) = lbuk[i];
        //printf("writing: %c\n",lbuk[i]/*pm2 + i*400 -1*/);
        //printf("hy\n");
       // *(pm2+400*i -1) = lbuk[i];
        printf("addr: %p, cont: %c\n", pm2+400*i , *(pm2+400*i));
        //fflush(hstdout);
    }
    
    //CloseHandle(hmem);
} 
