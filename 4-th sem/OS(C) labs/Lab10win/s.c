#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void main()
{
    char *pm;
    HANDLE hmem, hmtx, hstdout;//хендлы мьютексов и вывода
    hstdout = GetStdHandle(STD_OUTPUT_HANDLE);///получаем хендл вывода 
    
    hmem=CreateFileMapping((HANDLE)-1,NULL,  PAGE_READWRITE, 0, 16000, "SHAREMEM_MYY");//создаем общую память
    hmtx=CreateMutex(NULL, FALSE,"myMutex");//и мутекс на нее
    if (hmem==0)//проверка на успех
    {
        printf("Error AllocSharedMem with RC=%ld\n", GetLastError());//если не успех, тто ошибка
        getchar(); 
        exit(0); 
    }
    pm=MapViewOfFile(hmem, FILE_MAP_WRITE,0,0,0);//размечаем общую память
    printf("Mem addr %p\n", pm);//смотрим получившийся адрес
    if (pm==NULL)//снова проверяем ошбику
    {
        printf("Error Mapping SharedMem with RC=%ld\n", GetLastError());//енсли что - выходми
        getchar(); 
        exit(0);
    }
    STARTUPINFO cif;//тут все длля создания нового процесса
	ZeroMemory(&cif,sizeof(STARTUPINFO));
	PROCESS_INFORMATION pi;
    CreateProcess("b.exe","Child of main",NULL,NULL,TRUE,CREATE_NEW_CONSOLE,NULL,NULL,&cif,&pi);//создание самого процесса
    Sleep(8000);//сон
    WaitForSingleObject(hmtx, INFINITE);//ждем освобождения мьютексаы
    printf("writing to pm\n");//вывод сообщения об этом
    strcpy(pm, "Privet Shmara !!!"); //пишем в общую память
    ReleaseMutex(hmtx);//отпускаем мьютекс
    printf("writing complited\n"); ///пишем сообщение что все ок
    //Sleep(10000);
    UnmapViewOfFile(pm);//анмапинг памяти 
   
    char *pm2;
    
    pm2 = VirtualAlloc(NULL, 5000, MEM_COMMIT, PAGE_READWRITE );///другая область памяти не зависящая от первой
    printf("addr: %p\n", pm2);//выводим адрес
    *pm2 = 'e';//несколько проверок на запись в новую память
    printf("addr: %p, cont: %c\n", pm2, *pm2);
    *(pm2+400) = 'n';
    printf("addr: %p, cont: %c\n", pm2+400, *(pm2+400));
    if(pm2 == NULL)//проверка на ошибку бвыделения паммяти
    {
        printf("Error alocating mem %d", GetLastError());
        exit(0);
    }
    //QueryMem(pm2, 1000);
    char lbuk[] = "abcdefghijklmnopqrst";//буфер из котоого будут выводиться символы
    *(pm2+400) = lbuk[0];
    printf("addr: %p, cont: %c\n", pm2+400, *(pm2+400));
    printf("\n\n\n\n\n");
    for(int i = 0;  i < 15; i++)
    {
       // *(pm2+i*400 - 1) = lbuk[i];
        //printf("writing: %c\n",lbuk[i]/*pm2 + i*400 -1*/);
        //printf("hy\n");
        *(pm2+400*i) = lbuk[i];//указатель на элемент массива
        printf("addr: %p, cont: %c\n", pm2+400*i , *(pm2+400*i));//выводим его адрес
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
