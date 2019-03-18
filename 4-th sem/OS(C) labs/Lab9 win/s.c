#include <windows.h>
#include <process.h>
#include <stdio.h>
char lbuk[ ]="abcdefghijklmnoprqstuwxy";
HANDLE hstdout;
DWORD actlen;
CRITICAL_SECTION csec;

void procthread1(void *arg)
{
    int k, j;
    COORD pos;
    for (k=0; k<24; k++) 
    {
        pos.X=20; pos.Y=k+1;
        EnterCriticalSection(&csec);
        SetConsoleCursorPosition(hstdout,pos); // установить курсор в позицию (20,k+1)
        SetConsoleTextAttribute(hstdout,FOREGROUND_BLUE); // и синий цвет
        for (j=0; j<(int)arg; j++) printf("%c",lbuk[k]);
        LeaveCriticalSection(&csec);
        Sleep(800);
    }
}
void procthread2(void *arg)
{
    int k, j;
    COORD pos;
    for (k=0; k<24; k++) 
    {
        pos.X=40; pos.Y=k+1;
        EnterCriticalSection(&csec);
        SetConsoleCursorPosition(hstdout,pos); // установить курсор в позицию (40,k+1)
        SetConsoleTextAttribute(hstdout,FOREGROUND_GREEN); // и зеленый цвет
        for (j=0; j<(int)arg; j++) printf("%c",lbuk[k]);
        LeaveCriticalSection(&csec);
        Sleep(1300);
    }
}
void procthread3(void *arg)
{
    int k, j;
    COORD pos;
    for (k=0; k<24; k++) 
    {
        pos.X=60; pos.Y=k+1;
        EnterCriticalSection(&csec);
        SetConsoleCursorPosition(hstdout,pos); // установить курсор в позицию (60,k+1)
        SetConsoleTextAttribute(hstdout,FOREGROUND_RED); // и красный цвет
        for (j=0; j<(int)arg; j++) printf("%c",lbuk[k]);
        LeaveCriticalSection(&csec);
        Sleep(1100);
    }
}
void main()
{
    HANDLE hthread1, hthread2, hthread3;
    unsigned long threadid1, threadid2, threadid3;
    int k;
    COORD pos;
    hstdout=GetStdHandle(STD_OUTPUT_HANDLE);
    InitializeCriticalSection(&csec);
    hthread1=(HANDLE)_beginthreadNT(procthread1, 4096, (void *)2, NULL, 0,
    &threadid1);
    hthread2=(HANDLE)_beginthreadNT(procthread2, 4096, (void *)3, NULL, 0,
    &threadid2);
    hthread3=(HANDLE)_beginthreadNT(procthread3, 4096, (void *)4, NULL, 0,
    &threadid3);
    Sleep(600);
    for (k=0; k<24; k++) 
    {
        pos.X=1; pos.Y=k+1;
        EnterCriticalSection(&csec);
        SetConsoleCursorPosition(hstdout,pos); // установить курсор в позицию (10,k+1)
        SetConsoleTextAttribute(hstdout, // и белый цвет
        FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
        printf("%c++",lbuk[k]);
        LeaveCriticalSection(&csec);
        Sleep(1000); 
    }
    getchar();
    DeleteCriticalSection(&csec);
    CloseHandle(hthread1); CloseHandle(hthread2); CloseHandle(hthread3);
} 