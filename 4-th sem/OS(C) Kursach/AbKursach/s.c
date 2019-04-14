#include <windows.h>
#include <stdio.h>

HANDLE hstdout;
DWORD actlen;
CRITICAL_SECTION csec;

HANDLE hthrd;
unsigned long thredL;
HINSTANCE hInstance;
STARTUPINFO si;
int nCmdShow;
HWND hwnd;
HDC hdc;
MSG msg;
WNDCLASS wc;
int passChislo = 1;
int curDigit = 0;
int isRunning = 1;

HBRUSH brushGreen;
HBRUSH brushGray;
HBRUSH brushBlue;

HPEN pen;





void passajir(void *arg)
{

}

void Draw()
{
     
}


void main()
{
  /*  hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
    system("cls");

    printf("Vvedite colichestvo passajirov:\n");
    scanf("%d", &passChislo);*/

    LRESULT WINAPI WinProc(HWND hwnd, UINT tmsg, WPARAM wParam, LPARAM lParam)
    {
       switch(tmsg)
        {
            case WM_DESTROY:
            {
                PostQuitMessage(0);
                exit(0);
                return 0;
            }
            case WM_KEYDOWN: 
            {
                hdc = GetDC(hwnd); 
                char key[1]; 
                key[0] = (char)wParam;
                if(curDigit == -1)
                {
                    exit(0);
                }
                if(key[0] > '0' && key[0] < '9')
                {
                    TextOut(hdc, 128+64+16+curDigit*8, 32, key, 1); 
                    curDigit++;
                }else{
                    curDigit = -1;
                }
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

    brushGreen = CreateSolidBrush(RGB(128,256, 64));
    brushBlue = CreateSolidBrush(RGB(32,128,256));
    brushGray = CreateSolidBrush(RGB(128,128,128));
    pen = CreatePen(PS_SOLID, 1, RGB(0,0,0)); 


    char text1[] = "Vvedite chislo passajirov";
    TextOut(hdc, 32, 32, text1, sizeof(text1));

    while(GetMessage(&msg, 0, 0, 0) && curDigit > -1)
    {
        DispatchMessage(&msg);
    } 

    int x,y;
    while(1)
    {
        if(PeekMessage(&msg, hwnd,  0, 0, PM_REMOVE))
        {
                DispatchMessage(&msg);
        }

        Rectangle(hdc, x, y, x+8, y+8);
        x++;
        y++;
        Sleep(30);
    }
   

}