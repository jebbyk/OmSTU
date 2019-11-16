#include <windows.h>
#include <process.h>
#include <stdio.h>


HANDLE hstdout;//хендл вывода
CRITICAL_SECTION csec;//перменная для хранения инфомрмации о критической секции
char buf[12] = "123456789012";//первый буфер из которого выводятся символы
HANDLE hthreadRdr1, hthreadRdr2, hthreadRdr3, hthreadWrtr1, hthreadWrtr2, hthreadWrtr3, hWinThread;//хендлы для создания потоков
unsigned long rdrThrd1, rdrThrd2, rdrThrd3, wrtrThrd1, wrtrThrd2, wrtrThrd3, winThread;//переменные для хранения информации о потоках

HINSTANCE hInstance;//длля создания окна
STARTUPINFO si;//вспомогательная переменная для создания окна
int nCmdShow;
HWND hwnd;//хендлл окна
HDC hdc;//дискриптор рисовалки
MSG msg;//переменная хранящая сообщения окна
WNDCLASS wc;//класс окна


void procThreadWriter1(void *arg)//первый поток писатель
{
    char pat[12] = "abcdefghijkl";//его буфер из которого он выводит символы
    int k;
    for(k = 0; k < 32; k++)//цыкл
    {
        int i;
        EnterCriticalSection(&csec);//в нем указываем начало критической секции на время записи чтоб прцесс не мог быть прерван в процессе записи
        for(i = 0; i < 6; i++)
        {
            buf[i] = pat[i];//пишем в общий буфер первую половину символов
        }
        LeaveCriticalSection(&csec);//вызодим из критической секции

        Sleep(1);//засыпаем на время

        EnterCriticalSection(&csec);//снова входим в критическую секцию
        for(i = 6; i < 12; i++)
        {
            buf[i] = pat[i];//пишем в общий буфер вторую половину символов
        }
        LeaveCriticalSection(&csec);//покидаем критическую секцию

        Sleep(1);//снова засыпаем
    }
    EnterCriticalSection(&csec);//снова входим в критическую секцию
    COORD pos;//перменная для координат
    pos.X=80; pos.Y=4;//указываем их
    SetConsoleCursorPosition(hstdout,pos);//и перемещаем туда курсор
    printf("wirtiter1 ended");//пишем отладочную информацию
    LeaveCriticalSection(&csec);//выходим из критической секции
}

void procThreadWriter2(void *arg)//второй поток писатель (работает так же как первый, но у него дургой набор выводимых букв)
{
    char pat[12] = "ABCDEFGHIJKL";///овт он, другой
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

void procThreadWriter3(void *arg)//третий поток писатель
{
    char pat[12] = "-<+_!\%^&*\\|@";//тут уже третий набор букв
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


void procThreadReader1(void *arg)//поток читатель
{
    int j;
    for(j = 0; j < 32; j++)
    {
        //*
        COORD pos;//указываем координаты
        pos.X=20; pos.Y=j+1;//
        EnterCriticalSection(&csec);//входим в критическую секцию, в которой процесс не может быть прерван
        SetConsoleCursorPosition(hstdout,pos);//указываем положение курсора по указанным ранее координатам
        SetConsoleTextAttribute(hstdout,FOREGROUND_BLUE);//указываем цвет текста
        int i;
        for(i = 0; i < 12; i++)
        {
            printf("%c", buf[i]);//выводим буквы из общего буфера
        }
        LeaveCriticalSection(&csec);//покидаем критическую секцию
        //*/
        
        /*
        TextOut(hdc, 20*8, j*16, buf, 12);
        */
        Sleep(2);///засыпаем
    } 
}

void procThreadReader2(void *arg)//второй читатель работает так же как превый но с другим интервалом времени между попытками чтения
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

void procThreadReader3(void *arg)//третий читатель
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
    LRESULT WINAPI WinProc(HWND hwnd, UINT tmsg, WPARAM wParam, LPARAM lParam)//это для работы с окномч
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
    hstdout=GetStdHandle(STD_OUTPUT_HANDLE);//получаем окно вывода консоли
    system("cls");//предварительно очищаем ее
    InitializeCriticalSection(&csec);//создаем критическую секцию
    hthreadWrtr1 = (HANDLE)_beginthreadex(NULL, 4096, procThreadWriter1, (void *)2, 0,&wrtrThrd1);//запускаем все потоки
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
    getchar();//ждем любуемся
   
    DeleteCriticalSection(&csec);//удаляем критическую секцию

    CloseHandle(hthreadWrtr1); CloseHandle(hthreadWrtr2); CloseHandle(hthreadWrtr3);//закрываем треды
    CloseHandle(hthreadRdr1); CloseHandle(hthreadRdr2); CloseHandle(hthreadRdr3);//досвидаения
} 