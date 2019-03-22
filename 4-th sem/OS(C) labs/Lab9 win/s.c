#include <windows.h>
#include <process.h>
#include <stdio.h>


HANDLE hstdout;
DWORD actlen;
CRITICAL_SECTION csec;
char buf[12] = "123456789012";
HANDLE hthreadRdr1, hthreadRdr2, hthreadRdr3, hthreadWrtr1, hthreadWrtr2, hthreadWrtr3, hWinThread;
unsigned long rdrThrd1, rdrThrd2, rdrThrd3, wrtrThrd1, wrtrThrd2, wrtrThrd3, winThread;

HINSTANCE hInstance;
STARTUPINFO si;
int nCmdShow;
HWND hwnd;
HDC hdc;
MSG msg;
WNDCLASS wc;


void procThreadWriter1(void *arg)
{
    char pat[12] = "abcdefghijkl";
    int k;
    for(k = 0; k < 32; k++)
    {
        int i;
        EnterCriticalSection(&csec);
        for(i = 0; i < 6; i++)
        {
            buf[i] = pat[i];
        }
        LeaveCriticalSection(&csec);

        Sleep(1);

        EnterCriticalSection(&csec);
        for(i = 6; i < 12; i++)
        {
            buf[i] = pat[i];
        }
        LeaveCriticalSection(&csec);

        Sleep(1);
    }
    EnterCriticalSection(&csec);
    COORD pos;
    pos.X=80; pos.Y=4;
    SetConsoleCursorPosition(hstdout,pos);
    printf("wirtiter1 ended");
    LeaveCriticalSection(&csec);
}

void procThreadWriter2(void *arg)
{
    char pat[12] = "ABCDEFGHIJKL";
    int k;
    for(k = 0; k < 32; k++)
    {
        int i;
        EnterCriticalSection(&csec);
        for(i = 0; i < 6; i++)
        {
            buf[i] = pat[i];
        }
        LeaveCriticalSection(&csec);

       Sleep(2);

        EnterCriticalSection(&csec);
        for(i = 6; i < 12; i++)
        {
            buf[i] = pat[i];
        }
        LeaveCriticalSection(&csec);

       Sleep(2);
    }
    EnterCriticalSection(&csec);
    COORD pos;
    pos.X=80; pos.Y=6;
    SetConsoleCursorPosition(hstdout,pos);
    printf("wirtiter2 ended");
    LeaveCriticalSection(&csec);
}

void procThreadWriter3(void *arg)
{
    char pat[12] = "-<+_!\%^&*\\|@";
    int k;
    for(k = 0; k < 32; k++)
    {
        int i;
        EnterCriticalSection(&csec);
        for(i = 0; i < 6; i++)
        {
            buf[i] = pat[i];
        }
        LeaveCriticalSection(&csec);

        Sleep(3);

        EnterCriticalSection(&csec);
        for(i = 6; i < 12; i++)
        {
            buf[i] = pat[i];
        }
        LeaveCriticalSection(&csec);

        Sleep(3);
    }

    EnterCriticalSection(&csec);
    COORD pos;
    pos.X=80; pos.Y=8;
    SetConsoleCursorPosition(hstdout,pos);
    printf("wirtiter3 ended");
    LeaveCriticalSection(&csec);
}


void procThreadReader1(void *arg)
{
    int j;
    for(j = 0; j < 32; j++)
    {
        //*
        COORD pos;
        pos.X=20; pos.Y=j+1;
        EnterCriticalSection(&csec);
        SetConsoleCursorPosition(hstdout,pos);
        SetConsoleTextAttribute(hstdout,FOREGROUND_BLUE);
        int i;
        for(i = 0; i < 12; i++)
        {
            printf("%c", buf[i]);
        }
        LeaveCriticalSection(&csec);
        //*/
        
        /*
        TextOut(hdc, 20*8, j*16, buf, 12);
        */
        Sleep(2);
    } 
}

void procThreadReader2(void *arg)
{
    int j;
    for(j = 0; j < 32; j++)
    {
        //*
        COORD pos;
        pos.X=40; pos.Y=j+1;
        EnterCriticalSection(&csec);
        SetConsoleCursorPosition(hstdout,pos);
        SetConsoleTextAttribute(hstdout,FOREGROUND_RED);
        int i;
        for(i = 0; i < 12; i++)
        {
            printf("%c", buf[i]);
            
        }
        LeaveCriticalSection(&csec);
        //*/

        /*
        TextOut(hdc, 40*8, j*16, buf, 12);
        */
        Sleep(4);
    } 
}

void procThreadReader3(void *arg)
{
   
    int j;
    for(j = 0; j < 32; j++)
    {
        //*
        COORD pos;
        pos.X=60; pos.Y=j+1;
        EnterCriticalSection(&csec);
        SetConsoleCursorPosition(hstdout,pos);
        SetConsoleTextAttribute(hstdout,FOREGROUND_GREEN);
        int i;
        for(i = 0; i < 12; i++)
        {
            printf("%c", buf[i]);
        }
        LeaveCriticalSection(&csec);
        //*/

        /*
        TextOut(hdc, 60*8, j*16, buf, 12);
        */
        Sleep(6);
    } 
}

void main()
{
    
/*
    LRESULT WINAPI WinProc(HWND hwnd, UINT tmsg, WPARAM wParam, LPARAM lParam)
    {
       switch(tmsg)
        {
            case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            }
        }
        return DefWindowProc(hwnd, tmsg, wParam, lParam);
    }

    GetStartupInfo(&si);
    if(si.dwFlags & STARTF_USESHOWWINDOW)
    {
        nCmdShow = si.wShowWindow;
    }else{
        nCmdShow = SW_SHOWDEFAULT;
    }
    hInstance = GetModuleHandle(NULL);

    memset(&wc, 0, sizeof(wc));
    wc.lpszClassName = "MyClass";
    wc.lpfnWndProc = WinProc;
    wc.hCursor=LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);
    wc.hInstance = hInstance;
   
    if(!RegisterClass(&wc)) return;

    hwnd = CreateWindow("MyClass", "OurWindow", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, NULL);
    hdc = GetDC(hwnd);

    ShowWindow(hwnd, nCmdShow);
    */
    hstdout=GetStdHandle(STD_OUTPUT_HANDLE);
    system("cls");
    InitializeCriticalSection(&csec);
    hthreadWrtr1 = (HANDLE)_beginthreadex(NULL, 4096, procThreadWriter1, (void *)2, 0,&wrtrThrd1);
    hthreadWrtr2 = (HANDLE)_beginthreadex(NULL, 4096, procThreadWriter2, (void *)3, 0,&wrtrThrd2);
    hthreadWrtr3 = (HANDLE)_beginthreadex(NULL, 4096, procThreadWriter3, (void *)4, 0,&wrtrThrd3);

    hthreadRdr1 = (HANDLE)_beginthreadex(NULL, 4096, procThreadReader1, (void *)2, 0,&rdrThrd1);
    hthreadRdr2 = (HANDLE)_beginthreadex(NULL, 4096, procThreadReader2, (void *)3, 0,&rdrThrd2);
    hthreadRdr3 = (HANDLE)_beginthreadex(NULL, 4096, procThreadReader3, (void *)4, 0,&rdrThrd3);
/*
    
   while(GetMessage(&msg, 0, 0, 0))
   {
        DispatchMessage(&msg);
   }
 */  
    getchar();
   
    DeleteCriticalSection(&csec);

    CloseHandle(hthreadWrtr1); CloseHandle(hthreadWrtr2); CloseHandle(hthreadWrtr3);
    CloseHandle(hthreadRdr1); CloseHandle(hthreadRdr2); CloseHandle(hthreadRdr3);
} 