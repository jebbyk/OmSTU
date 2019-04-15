#include <windows.h>
#include <stdio.h>

HANDLE hstdout;
DWORD actlen;
CRITICAL_SECTION csec;

HINSTANCE hInstance;
STARTUPINFO si;
int nCmdShow;
HWND hwnd;
HDC hdc;
MSG msg;
WNDCLASS wc;
char passChisloC[4] = "";
int passChislo = 1;
int curDigit = 0;
int isRunning = 1;

HBRUSH brushGreen;
HBRUSH brushGray;
HBRUSH brushBlue;
HBRUSH brushBlack;
HBRUSH brushLightBlue;
HBRUSH brushWhite;

HPEN pen;

POINT samoletForma[] = {
    {0,24},
    {2, 22},
    {4, 5},
    {20,-2},
    {20,-4},
    {4, -2},
    {2, -20},
    {-2, -20},
    {-4, -2},
    {-20,-4},
    {-20,-2},
    {-4, 5},
    {-2, 22}
};
POINT samoletPos;
POINT samoletReis;
float samoletSpeed = 0.5;
BOOL samoletStop;
BOOL samoletMove;
int samoletPass;
float samoletUgol;



POINT gorod1Pos = { 128, 128};
POINT gorod2Pos = { 1000, 200};
POINT gorod3Pos = { 256, 600};
POINT gorod4Pos = { 900, 580};

POINT oblast1Forma[] = {
    {-100, 0},
    {-80, -20},
    {-30, -25},
    {-30, -60},
    {0, -100},
    {20, -50},
    {60, -70},
    {60, -20},
    {80, 0},
    {70, 20},
    {40, 40},
    {0, 80},
    {-20, 100},
    {-30, 50},
    {-80, 10}
    };
POINT gorod1Forma[] = {
    {-30, 40},
    {-20, 46},
    {10, 42},
    {14, 32},
    {22, 32},
    {24, 38},
    {44, 18},
    {44, 6},
    {18, 8},
    {18, -8},
    {6, -18},
    {0, -20},
    {-10, 16},
    {24, -20},
    {-34, 0},
    {-20, 12},
    {-22, 20},
    {-36, 24},
    {-44, 26},
    {-44, 34}
    };

POINT oblast2Forma[] = {
        {7,168},
        {-32,175},
        {-55, 148},
        {-90,139},
        {-100,100},
        {-85,80},
        {-83, 52},
        {-93,22},
        {-65,-11},
        {-59,-26},
        {-62,67},
        {-67,-92},
        {-17,-107},
        {20, -75},
        {56, -89},
        {58, -40},
        {33, -21},
        {42, 14},
        {60, 31},
        {58, 86},
        {34,107},
        {25, 140},
        {56, 166},
        {26, 180}
    };
POINT gorod2Forma[] = {
    {-30, 40},
    {-20, 46},
    {10, 42},
    {14, 32},
    {22, 32},
    {24, 38},
    {44, 18},
    {44, 6},
    {18, 8},
    {18, -8},
    {6, -18},
    {0, -20},
    {-10, 16},
    {24, -20},
    {-34, 0},
    {-20, 12},
    {-22, 20},
    {-36, 24},
    {-44, 26},
    {-44, 34}
    };









POINT oblast3Forma[] = {
        {-40, 45},
        {-42, 91},
        {-2, 91},
        {42, 44},
        {93, 38},
        {120, -27},
        {104, -65},
        {99, -110},
        {74, -108},
        {70, -70},
        {47, -33},
        {-16, -48},
        {-41, -35},
        {-74, 21}
    };
POINT gorod3Forma[] = {
        {-30, 40},
        {-20, 46},
        {10, 42},
        {14, 32},
        {22, 32},
        {24, 38},
        {44, 18},
        {44, 6},
        {18, 8},
        {18, -8},
        {6, -18},
        {0, -20},
        {-10, 16},
        {24, -20},
        {-34, 0},
        {-20, 12},
        {-22, 20},
        {-36, 24},
        {-44, 26},
        {-44, 34}
    };

POINT oblast4Forma[] = {
        {-67, 27},
        {-29, 28},
        {-65, 81},
        {-22, 57},
        {6, 80},
        {35, 62},
        {36, 22},
        {84, 16},
        {46, -20},
        {62, -36},
        {80, -80},
        {40, -80},
        {16, -46},
        {-20, -44},
        {-40, -80},
        {-80, -82},
        { -100, -56},
        {-100, -16},
        {-94, 20},
        {-82, 68}        
    };
POINT gorod4Forma[] = {
        {-30, 40},
        {-20, 46},
        {10, 42},
        {14, 32},
        {22, 32},
        {24, 38},
        {44, 18},
        {44, 6},
        {18, 8},
        {18, -8},
        {6, -18},
        {0, -20},
        {-10, 16},
        {24, -20},
        {-34, 0},
        {-20, 12},
        {-22, 20},
        {-36, 24},
        {-44, 26},
        {-44, 34}
    };

HANDLE hmtxShow;

typedef struct {
    int x;
    int y;
    int cx;
    int cy;
    BOOL walkAeroport;
    BOOL walkGorod;
    BOOL samolet;
    BOOL son;
    int sonDlinna;
}passajir;

passajir passajiry[8192];

void passajirFunction(void *arg)
{
    int i = (int)arg;
    if(passajiry[i].walkAeroport == TRUE)
    {
        
    }

    int i = (int)arg;
    while(1)
    {
        passajiry[i].x++;
        passajiry[i].y++;


        WaitForSingleObject(hmtxShow, INFINITE);
        Rectangle(hdc, passajiry[i].x-4, passajiry[i].y-4, passajiry[i].x+4, passajiry[i].y+4);
        ReleaseMutex(hmtxShow);

        /*int s,h = 1024;
        for(int n = 1; n < h; n++)
        {
            for(int m = 1; m < h; m++ )
            {
                s = m*n + n*m - n%m * n/m;
            }
        }*/

        Sleep(25);
    }
}


void paintFunction()
{
    Rectangle(hdc, 0,0,1280,720);

    HRGN oblast1 = CreatePolygonRgn(oblast1Forma, sizeof(oblast1Forma)/8, WINDING);
    FillRgn(hdc, oblast1, brushGreen);
    HRGN gorod1 = CreatePolygonRgn(gorod1Forma, sizeof(gorod1Forma)/8, WINDING);
    FillRgn(hdc, gorod1, brushGray);

    HRGN oblast2 = CreatePolygonRgn(oblast2Forma, sizeof(oblast2Forma)/8, WINDING);
    FillRgn(hdc, oblast2, brushGreen);
    HRGN gorod2 = CreatePolygonRgn(gorod2Forma, sizeof(gorod2Forma)/8, WINDING);
    FillRgn(hdc, gorod2, brushGray);

    HRGN oblast3 = CreatePolygonRgn(oblast3Forma, sizeof(oblast3Forma)/8, WINDING);
    FillRgn(hdc, oblast3, brushGreen);
    HRGN gorod3 = CreatePolygonRgn(gorod3Forma, sizeof(gorod3Forma)/8, WINDING);
    FillRgn(hdc, gorod3, brushGray);

    HRGN oblast4 = CreatePolygonRgn(oblast4Forma, sizeof(oblast4Forma)/8, WINDING);
    FillRgn(hdc, oblast4, brushGreen);
    HRGN gorod4 = CreatePolygonRgn(gorod4Forma, sizeof(gorod4Forma)/8, WINDING);
    FillRgn(hdc, gorod4, brushGray);

    HRGN samolet = CreatePolygonRgn(samoletForma, sizeof(samoletForma)/8,WINDING );
    FillRgn(hdc, samolet, brushWhite);

    
    for(int i = 0; i < passChislo; i++)
    {
        Rectangle(hdc, passajiry[i].x-4, passajiry[i].y-4, passajiry[i].x+4, passajiry[i].y+4);
    }
}





void main()
{
    hmtxShow = CreateMutex(NULL, FALSE, NULL);
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
                if(key[0] >= '0' && key[0] <= '9')
                {
                    TextOut(hdc, 128+64+16+curDigit*8, 32, key, 1); 
                    passChisloC[curDigit] = key[0];
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
    hwnd = CreateWindow("MyClass", "OurWindow", WS_OVERLAPPEDWINDOW, 0, 0, 1280, 720, 0, 0, hInstance, NULL);
    hdc = GetDC(hwnd);
    ShowWindow(hwnd, nCmdShow);

    brushGreen = CreateSolidBrush(RGB(64,128, 32));
    brushBlue = CreateSolidBrush(RGB(32,128,255));
    brushGray = CreateSolidBrush(RGB(128,128,128));
    brushWhite = CreateSolidBrush(RGB(200,200,200));
    pen = CreatePen(PS_SOLID, 1, RGB(0,0,0)); 


    char text1[] = "Vvedite chislo passajirov";
    TextOut(hdc, 32, 32, text1, sizeof(text1));

    while(GetMessage(&msg, 0, 0, 0) && curDigit > -1)
    {
        DispatchMessage(&msg);
    }

    sscanf(passChisloC, "%d", &passChislo);


  
    for(int i = 0; i < sizeof(oblast1Forma)/8; i++)
    {
        oblast1Forma[i].x += gorod1Pos.x;
        oblast1Forma[i].y += gorod1Pos.y;
    }
    for(int i = 0; i < sizeof(gorod1Forma)/8; i++)
    {
        gorod1Forma[i].x += gorod1Pos.x;
        gorod1Forma[i].y += gorod1Pos.y;
    }
     for(int i = 0; i < sizeof(oblast2Forma)/8; i++)
    {
        oblast2Forma[i].x += gorod2Pos.x;
        oblast2Forma[i].y += gorod2Pos.y;
    }
    for(int i = 0; i < sizeof(gorod2Forma)/8; i++)
    {
        gorod2Forma[i].x += gorod2Pos.x;
        gorod2Forma[i].y += gorod2Pos.y;
    }
    for(int i = 0; i < sizeof(oblast3Forma)/8; i++)
    {
        oblast3Forma[i].x += gorod3Pos.x;
        oblast3Forma[i].y += gorod3Pos.y;
    }
    for(int i = 0; i < sizeof(gorod3Forma)/8; i++)
    {
        gorod3Forma[i].x += gorod3Pos.x;
        gorod3Forma[i].y += gorod3Pos.y;
    }
    for(int i = 0; i < sizeof(oblast4Forma)/8; i++)
    {
        oblast4Forma[i].x += gorod4Pos.x;
        oblast4Forma[i].y += gorod4Pos.y;
    }
    for(int i = 0; i < sizeof(gorod4Forma)/8; i++)
    {
        gorod4Forma[i].x += gorod4Pos.x;
        gorod4Forma[i].y += gorod4Pos.y;
    }

    for(int i = 0; i < sizeof(samoletForma)/8; i++)
    {
        samoletForma[i].x += 650;
        samoletForma[i].y += 320;
    }



    for(int i = 0; i < passChislo; i++)
    {
        passajir pas1;
        pas1.x = 1000/passChislo * i ;
        pas1.y = 0;
        passajiry[i] = pas1;
        HANDLE hthr;
        unsigned long lthr;
        hthr = (HANDLE)_beginthread(passajirFunction, 0,(void*)i);
    }
    

    while(1)
    {
        if(PeekMessage(&msg, hwnd,  0, 0, PM_REMOVE))
        {
            DispatchMessage(&msg);
        }

        paintFunction();

        Sleep(50);
    }
   

}