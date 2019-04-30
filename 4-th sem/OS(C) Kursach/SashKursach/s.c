#include <windows.h>
#include <stdio.h>
#include <math.h>

HANDLE hstdout;//хендл стандартного вывода

HINSTANCE hInstance;//для создания окна
STARTUPINFO si;//информация о старте окна
int nCmdShow;
HWND hwnd;//дескриптор окна
HDC hdc;//дескриптор для рисования
MSG msg;//сообщение передаваемое окном
WNDCLASS wc;//для создания окна

HBRUSH brushGreen;//кисти для заливки разными цветами
HBRUSH brushRed;
HBRUSH brushBlack;
HBRUSH brushGray;

HPEN pen;

HANDLE hmtx;

typedef struct{
    float x;
    float y;
}coord;
typedef struct {
    float x;
    float y;
    short type;//1 - перекресток 0 - поворот
    short open;// 0 - закртыо 1 - все открыто
}wpoint;

float s = 0.80;
wpoint wpoints[] =
{
    {100, 300, 0, 1},//0
    {300, 300, 1, 1},//1
    {400, 300, 0, 1},//2
    {400, 400, 1, 1},//3
    {400, 500, 1, 1},//4
    {400, 600, 0, 1},//5
    {300, 600, 1, 1},//6
    {100, 600, 0, 1},//7
    {300, 100, 0, 1},//8
    {600, 100, 0, 1},//9
    {600, 300, 1, 1},//10
    {600, 400, 0, 1},//11
    {500, 400, 1, 1},//12
    {300, 400, 0, 1},//13
    {500, 300, 0, 1},//14
    {800, 300, 0, 1},//15
    {800, 600, 0, 1},//16
    {600, 600, 1, 1},//17
    {500, 600, 0, 1},//18
    {500, 500, 1, 1},//19
    {300, 500, 0, 1},//20
    {600, 500, 0, 1},//21
    {600, 800, 0, 1},//22
    {300, 800, 0, 1}//23

};

HANDLE mutexes[24];

typedef struct{
    short wpoints[8];
}road;

road roads[4];
void ConstructRoads()
{
    road r1;
    r1.wpoints[0] = 0;
    r1.wpoints[1] = 1;
    r1.wpoints[2] = 2;
    r1.wpoints[3] = 3;
    r1.wpoints[4] = 4;
    r1.wpoints[5] = 5;
    r1.wpoints[6] = 6;
    r1.wpoints[7] = 7;
    road r2;
    r2.wpoints[0] = 8;
    r2.wpoints[1] = 9;
    r2.wpoints[2] = 10;
    r2.wpoints[3] = 11;
    r2.wpoints[4] = 12;
    r2.wpoints[5] = 3;
    r2.wpoints[6] = 13;
    r2.wpoints[7] = 1;
    road r3;
    r3.wpoints[0] = 14;
    r3.wpoints[1] = 10;
    r3.wpoints[2] = 15;
    r3.wpoints[3] = 16;
    r3.wpoints[4] = 17;
    r3.wpoints[5] = 18;
    r3.wpoints[6] = 19;
    r3.wpoints[7] = 12;
    road r4;
    r4.wpoints[0] = 20;
    r4.wpoints[1] = 4;
    r4.wpoints[2] = 19;
    r4.wpoints[3] = 21;
    r4.wpoints[4] = 17;
    r4.wpoints[5] = 22;
    r4.wpoints[6] = 23;
    r4.wpoints[7] = 6;

    roads[0] = r1;
    roads[1] = r2;
    roads[2] = r3;
    roads[3] = r4;
}

typedef struct {
    float x;
    float y;
    float tx;
    float ty;
    float ox;
    float oy;
    float spd;
    float maxSpd;
    short dir;
    short state;
    short r;
    short curP;
}car;

car cars[4];



void CarThread(void* _i)
{
    short i = _i;
    while(1)
    {
        if(cars[i].state == 0)
        {
            coord dir_vector = {cars[i].tx-cars[i].x, cars[i].ty-cars[i].y};//рассчитываем вектор для движеня персонажа
            float l = sqrt(pow(dir_vector.x,2)+pow(dir_vector.y, 2));//рассчитываем длину этого вектора
            coord normal_v  = dir_vector;//нормализованный вектор, который должен иметь то же направление, но длинну 1
            normal_v.x /= l;//для этого делим его на длину первого вектора
            normal_v.y /= l;
            short nextP = cars[i].curP + 1;
            if (nextP == 8) nextP = 0;
            if(wpoints[roads[cars[i].r].wpoints[nextP]].open == 0)
            {
                if(cars[i].spd < cars[i].maxSpd && cars[i].spd < l/40)//если скорость персонажа меньше 0.5 и в принципе достаточно млаа
                {
                    cars[i].spd += cars[i].maxSpd/40;//то постеменно ее увеличиваем
                }
                if(cars[i].spd > l/40)//если скорость слишком велика по отношению к оставшемуся расстоянию до цели, 
                {
                    cars[i].spd = l/40;//то снижаем ее
                }
            }else{
                if(cars[i].spd < cars[i].maxSpd &&  cars[i].spd < l)//если скорость персонажа меньше 0.5 и в принципе достаточно млаа
                {
                    cars[i].spd += cars[i].maxSpd/40;//то постеменно ее увеличиваем
                    //if(cars[i].spd > l) cars[i].spd = l;
                }
            }

            cars[i].x += normal_v.x * cars[i].spd;//изменяем координаты персонажа на основании его скорости и нормализованного вектора длвижения
            cars[i].y += normal_v.y * cars[i].spd;

           // if(wpoints[roads[cars[i].r].wpoints[cars[i].curP]].type == 1)
            //{
                coord old_vector = {cars[i].ox-cars[i].x, cars[i].oy-cars[i].y};//рассчитываем вектор для движеня персонажа
                float ol = sqrt(pow(old_vector.x,2)+pow(old_vector.y, 2));
                if(ol > 64) {
                    ReleaseMutex(mutexes[roads[cars[i].r].wpoints[cars[i].curP]]);
                    wpoints[roads[cars[i].r].wpoints[cars[i].curP]].open = 1;
                }
           // }



            if(l < 2)//если персонаж остановился
            {
                if(wpoints[roads[cars[i].r].wpoints[nextP]].open == 0) cars[i].spd = 0;
                cars[i].state = 1;
                cars[i].curP++;
                if(cars[i].curP == 8) cars[i].curP = 0;
            }
        }
        if(cars[i].state == 1)
        {
            
            short nextP = cars[i].curP + 1;
            if(nextP == 8) nextP = 0;
            
           // if(wpoints[roads[cars[i].r].wpoints[cars[i].curP]].type == 1)
            //{
                WaitForSingleObject(mutexes[roads[cars[i].r].wpoints[cars[i].curP]], INFINITE);
                wpoints[roads[cars[i].r].wpoints[cars[i].curP]].open = 0;
           /// }
            

            coord dir_vector = {wpoints[roads[cars[i].r].wpoints[nextP]].x-cars[i].x, wpoints[roads[cars[i].r].wpoints[nextP]].y-cars[i].y};//рассчитываем вектор для движеня персонажа
            if(dir_vector.x * dir_vector.x > dir_vector.y * dir_vector.y)
            {
                if(dir_vector.x > 0) cars[i].dir = 0;
                else cars[i].dir = 2;
            }
            else{
                if(dir_vector.y > 0) cars[i].dir = 1;
                else cars[i].dir = 3;
            }

            switch(cars[i].dir)
            {
                case 0:
                cars[i].tx =  wpoints[roads[cars[i].r].wpoints[nextP]].x -32;//точки остановки перед перекрестками
                cars[i].ty =  wpoints[roads[cars[i].r].wpoints[nextP]].y;   
                break;
                case 1:
                cars[i].tx =  wpoints[roads[cars[i].r].wpoints[nextP]].x;
                cars[i].ty =  wpoints[roads[cars[i].r].wpoints[nextP]].y -32;   
                break;
                case 2:
                cars[i].tx =  wpoints[roads[cars[i].r].wpoints[nextP]].x + 32;
                cars[i].ty =  wpoints[roads[cars[i].r].wpoints[nextP]].y;   
                break;
                case 3:
                cars[i].tx =  wpoints[roads[cars[i].r].wpoints[nextP]].x;
                cars[i].ty =  wpoints[roads[cars[i].r].wpoints[nextP]].y + 32;   
                break;
            }

            cars[i].state = 0;
            cars[i].ox =  wpoints[roads[cars[i].r].wpoints[cars[i].curP]].x;
            cars[i].oy =  wpoints[roads[cars[i].r].wpoints[cars[i].curP]].y;
        }
        Sleep(1000/120);
    }

}

void SetCars(){
    for(int i = 0; i < 4; i++)
    {
        cars[i].r = i;
        cars[i].x = wpoints[roads[cars[i].r].wpoints[0]].x;
        cars[i].y = wpoints[roads[cars[i].r].wpoints[0]].y;
        cars[i].tx = wpoints[roads[cars[i].r].wpoints[1]].x;
        cars[i].ty = wpoints[roads[cars[i].r].wpoints[1]].y;
        cars[i].state = 0;
        cars[i].spd = 0;
        int r = rand()%40;

        cars[i].maxSpd = 0.1 + r/10.0f;
        cars[i].dir = 0;
        cars[i].curP = 0;
        HANDLE hthr;
        hthr = (HANDLE)_beginthread(CarThread, 0,(void*)i);
    }
}




void Draw()
{
    SelectObject(hdc, brushBlack);
    Rectangle(hdc, 0,0,800,800);//монотонная заливка всего экрана
    for(int i = 0; i < 4; i++)
    {
        SelectObject(hdc, brushGray);
        if(cars[i].dir == 0 || cars[i].dir == 2) Rectangle(hdc, cars[i].x*s - 16, cars[i].y*s - 8, cars[i].x*s + 16, cars[i].y*s + 8);
        if(cars[i].dir == 1 || cars[i].dir == 3)  Rectangle(hdc, cars[i].x*s - 8, cars[i].y*s - 16, cars[i].x*s + 8, cars[i].y*s + 16);
        

        for(int j = 0; j < 8; j++)
        {
            SelectObject(hdc, pen);
            if(j == 0)
            {
                MoveToEx(hdc, wpoints[roads[i].wpoints[j]].x*s, wpoints[roads[i].wpoints[j]].y*s, NULL);
            }
            else{
                LineTo(hdc, wpoints[roads[i].wpoints[j]].x*s, wpoints[roads[i].wpoints[j]].y*s);
            }
            if(j == 7) LineTo(hdc, wpoints[roads[i].wpoints[0]].x*s, wpoints[roads[i].wpoints[0]].y*s);

           // if(wpoints[roads[i].wpoints[j]].type == 1) {
                if(wpoints[roads[i].wpoints[j]].open == 1)
                {
                    SelectObject(hdc, brushGreen);
                    Rectangle(hdc, wpoints[roads[i].wpoints[j]].x*s - 8, wpoints[roads[i].wpoints[j]].y*s - 8, wpoints[roads[i].wpoints[j]].x*s + 8, wpoints[roads[i].wpoints[j]].y*s + 8);
                }else{
                    SelectObject(hdc, brushRed);
                    Rectangle(hdc, wpoints[roads[i].wpoints[j]].x*s - 8, wpoints[roads[i].wpoints[j]].y*s - 8, wpoints[roads[i].wpoints[j]].x*s + 8, wpoints[roads[i].wpoints[j]].y*s + 8);
                }
            //}
        }
    }
}



void main()
{
    
    for(int i = 0; i < 24; i++)
    {
        mutexes[i] = CreateMutex(NULL, FALSE, NULL);//мьютеексы для всех перекрестков
    }
    LRESULT WINAPI WinProc(HWND hwnd, UINT tmsg, WPARAM wParam, LPARAM lParam)//создание окна и считывание его событий
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
    hwnd = CreateWindow("MyClass", "OurWindow", WS_OVERLAPPEDWINDOW, 0, 0, 800, 800, 0, 0, hInstance, NULL);
    hdc = GetDC(hwnd);
    ShowWindow(hwnd, nCmdShow);


    brushGreen = CreateSolidBrush(RGB(0,255, 0));
    brushRed = CreateSolidBrush(RGB(255,0, 0));
    brushBlack = CreateSolidBrush(RGB(32,32, 32));
    brushGray = CreateSolidBrush(RGB(128,128,128));
    pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));

    ConstructRoads();
    SetCars();


    while(1)
    {
        Draw();
        if(PeekMessage(&msg, hwnd,  0, 0, PM_REMOVE))//мониторим события окна
        {
            DispatchMessage(&msg);
        }
        Sleep(1000/30);
    }
}