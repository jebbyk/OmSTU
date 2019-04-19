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
char voditeliChisloC[4] = "";//строка куда вводится число пассажиров
int voditeliChislo = 1;//число в котором преобразуется число пасиков из строки в цифру
int curDigit = 0;//для вода числа пасиков тукущий вводимый разряд числа)

HBRUSH brushGreen;//кисти для заливки разными цветами
HBRUSH brushGray;
HBRUSH brushBlue;
HBRUSH brushBlack;
HBRUSH brushLightBlue;
HBRUSH brushWhite;

HPEN pen;//карандаш для рисования линий


int tScorost  = 30;//скорость и плавность анимации

char text[12] = "";

POINT kolonka1Pos = {128, 256};
POINT kolonka2Pos = {256, 256};
POINT kolonka3Pos = {384, 256};
int kolonka1;
int kolonka2;
int kolonka3;
POINT kassaPos = {512,256};
int kassaLudi = 0;

    
HANDLE hmtx;



typedef struct {//структура с данными об отдельном водителе
    float x;//точные координаты (тоже для перемещения)
    float y;
    float mx;
    float my;
    int cx;
    int cy;
    int walkKassa;//к кассе
    int walkMachina;//к машине
    int machina;//в машине
    int ojidanie;// ждет 
    int son;//спит
    int driveKolonka;//городо в котором пассажир находится
    float skorost;
    int curKolonka;
}voditel;

voditel voditeli[1024];//массив с пассажирами

void voditelFunction(void *arg)//функция пассажира (с ней тред запускается)
{
    srand(arg);
    int i = (int)arg;//передаем аргумент, по которому определяем какой именно элемент массива модифицеровать
    voditeli[i].curKolonka = rand()%3;
    if(voditeli[i].curKolonka==0)
    {
        voditeli[i].cx = kolonka1Pos.x; //указываем начальные целевые координаты
        voditeli[i].cy = kolonka1Pos.y;
    }
    if(voditeli[i].curKolonka==1)
    {
        voditeli[i].cx = kolonka1Pos.x; //указываем начальные целевые координаты
        voditeli[i].cy = kolonka1Pos.y;
    }
    if(voditeli[i].curKolonka==2)
    {
        voditeli[i].cx = kolonka1Pos.x; //указываем начальные целевые координаты
        voditeli[i].cy = kolonka1Pos.y;
    }
    voditeli[i].x = voditeli[i].cx;
    voditeli[i].y = 800 + rand()%800;
    voditeli[i].driveKolonka = 1;//город


    while(1)
    {
        if(voditeli[i].driveKolonka == 1)
        {
            int loop = 0;
            if(voditeli[i].curKolonka == 0 && kolonka1 == 0) loop = 1;
            if(voditeli[i].curKolonka == 1 && kolonka2 == 0) loop = 1;
            if(voditeli[i].curKolonka == 2 && kolonka3  == 0) loop = 1;
            if(loop == 1)
            {
                float dx;
                float dy;
                dx = voditeli[i].cx - voditeli[i].x;
                dy = voditeli[i].cy - voditeli[i].y;
                
                float l = sqrt(dx*dx + dy*dy);
                dx /= l;
                dy /= l;//перемещение по вектору

                if(voditeli[i].skorost < l/10)
                {
                    voditeli[i].skorost++;//ускорение до опр предела
                }else{
                    voditeli[i].skorost = l/10;
                }
                if(voditeli[i].skorost > 10)
                {
                    voditeli[i].skorost = 10;//торможение если близко к цели
                }
                


                voditeli[i].x += dx*voditeli[i].skorost;
                voditeli[i].y += dy*voditeli[i].skorost;
                voditeli[i].mx = voditeli[i].x;
                voditeli[i].my = voditeli[i].y;


                if(voditeli[i].x > voditeli[i].cx -1 && voditeli[i].x < voditeli[i].cx + 1 &&        voditeli[i].y > voditeli[i].cy -1 && voditeli[i].y < voditeli[i].cy + 1)
                {
                    WaitForSingleObject(hmtx,INFINITE);
                    kassaLudi++;
                    voditeli[i].cx = kassaPos.x;
                    voditeli[i].cy = kassaPos.y + 4 + kassaLudi * 4;
                    voditeli[i].walkKassa = 1;//меняем его состояние на ожидание, если он очень близко к целевой точке
                
                    voditeli[i].driveKolonka = 0;
                    voditeli[i].skorost = 0;
                    ReleaseMutex(hmtx);
                }
            }

        }
        if(voditeli[i].walkKassa == 1)//если пассажир сейчас идет в аеропрт
        {
            float dx;
            float dy;
            dx = voditeli[i].cx - voditeli[i].x;//берем вектор по направлению к целевой точке
            dy = voditeli[i].cy - voditeli[i].y;
            
            float l = sqrt(dx*dx + dy*dy);// узнаем рассатояние
            if(l > 1.0f)
            {
                dx /= l;//нормализуем вектор
                dy /= l;
            }

            voditeli[i].x += dx;//перемещаем человека
            voditeli[i].y += dy;

            if(voditeli[i].x > voditeli[i].cx -1 && voditeli[i].x < voditeli[i].cx + 1 &&        voditeli[i].y > voditeli[i].cy -1 && voditeli[i].y < voditeli[i].cy + 1)
            {
                voditeli[i].walkKassa = 0;//меняем его состояние на ожидание, если он очень близко к целевой точке
                voditeli[i].ojidanie = 1;
            }

        }
        if(voditeli[i].ojidanie == 1)//если сейчас ожидание
        {
            float dx;
            float dy;
            dx = kassaPos.x - voditeli[i].x;
            dy = kassaPos.y - voditeli[i].y;
            float l = sqrt(dx*dx + dy*dy);//так же беерм вектор до самолета и узнаем расстояние
            if(l < 6.0f )//если оно очень маленькое то пытаемся сесть в самолет обязательно блокируя всем остальным людям доступ к самолету
            {
                voditeli[i].ojidanie = 0;
                voditeli[i].son = 1;
                Sleep(2000);
                for(int n = 0; n < voditeliChislo; n++)
                {
                    if(voditeli[n].ojidanie == 1)
                    {
                        voditeli[n].y -= 4;
                    }
                }
                WaitForSingleObject(hmtx, INFINITE);
                kassaLudi--;
                ReleaseMutex(hmtx);
                voditeli[i].son = 0;
                voditeli[i].walkMachina = 1;
                voditeli[i].cx = voditeli[i].mx;
                voditeli[i].cx = voditeli[i].mx;
            }
        }
        if(voditeli[i].walkMachina == 1)//если идем в город
        {
            float dx;
            float dy;
            dx = voditeli[i].cx - voditeli[i].x;//такое же перемещение по вектору до точки
            dy = voditeli[i].cy - voditeli[i].y;
            
            float l = sqrt(dx*dx + dy*dy);
            if(l > 1.0f)
            {
                dx /= l;
                dy /= l;
            }

            voditeli[i].x += dx;
            voditeli[i].y += dy;

            if(voditeli[i].x > voditeli[i].cx -1 && voditeli[i].x < voditeli[i].cx + 1 &&        voditeli[i].y > voditeli[i].cy -1 && voditeli[i].y < voditeli[i].cy + 1)
            {
                voditeli[i].walkMachina = 0;
                voditeli[i].machina = 1;
                voditeli[i].cx = voditeli[i].x;
                voditeli[i].cy = -100;
            }
        }
        if(voditeli[i].machina == 1)//если пассажир в самолете
        {
            float dx;
            float dy;
            dx = voditeli[i].cx - voditeli[i].x;
            dy = voditeli[i].cy - voditeli[i].y;
            
            float l = sqrt(dx*dx + dy*dy);
            dx /= l;
            dy /= l;//перемещение по вектору

            if(voditeli[i].skorost < l/10)
            {
                voditeli[i].skorost++;//ускорение до опр предела
            }else{
                voditeli[i].skorost = l/10;
            }
            if(voditeli[i].skorost > 10)
            {
                voditeli[i].skorost = 10;//торможение если близко к цели
            }
            
            voditeli[i].x += dx*voditeli[i].skorost;
            voditeli[i].y += dy*voditeli[i].skorost;
            voditeli[i].mx = voditeli[i].x;
            voditeli[i].my = voditeli[i].y;


            if(voditeli[i].x > voditeli[i].cx -1 && voditeli[i].x < voditeli[i].cx + 1 &&        voditeli[i].y > voditeli[i].cy -1 && voditeli[i].y < voditeli[i].cy + 1)
            {
                Sleep(2000);
                voditeli[i].driveKolonka = 1;
                voditeli[i].curKolonka = rand()%3;
                if(voditeli[i].curKolonka==0)
                {
                    voditeli[i].cx = kolonka1Pos.x; //указываем начальные целевые координаты
                    voditeli[i].cy = kolonka1Pos.y;
                }
                if(voditeli[i].curKolonka==1)
                {
                    voditeli[i].cx = kolonka1Pos.x; //указываем начальные целевые координаты
                    voditeli[i].cy = kolonka1Pos.y;
                }
                if(voditeli[i].curKolonka==2)
                {
                    voditeli[i].cx = kolonka1Pos.x; //указываем начальные целевые координаты
                    voditeli[i].cy = kolonka1Pos.y;
                }
                voditeli[i].machina = 0;
                voditeli[i].cx = voditeli[i].x;
                voditeli[i].cy = -100;
            }

        }
        Sleep(1000/tScorost);//задержка между отедльными кадрами анимации
    }
}


void paintFunction()//фнукция рисования
{
    Rectangle(hdc, 0,0,800,800);//монотонная заливка всего экрана

    Rectangle(hdc, kolonka1Pos.x + 16, kolonka1Pos.y - 8, kolonka1Pos.x  + 32, kolonka1Pos.y + 8 );
    Rectangle(hdc, kolonka2Pos.x + 16, kolonka2Pos.y - 8, kolonka2Pos.x  + 32, kolonka2Pos.y + 8 );
    Rectangle(hdc, kolonka3Pos.x + 16, kolonka3Pos.y - 8, kolonka3Pos.x  + 32, kolonka3Pos.y + 8 );

    Rectangle(hdc, kassaPos.x + 16, kassaPos.y - 8, kassaPos.x  + 32, kassaPos.y + 8 );
    
    
    for(int i = 0; i < voditeliChislo; i++)
    {
        Rectangle(hdc, voditeli[i].mx-8, voditeli[i].my-16, voditeli[i].mx+8, voditeli[i].my+16);//рисуем маленьких человечков
        Rectangle(hdc, voditeli[i].x-2, voditeli[i].y-2, voditeli[i].x+2, voditeli[i].y+2);//рисуем маленьких человечков
    }
}





void main()
{
    hmtx = CreateMutex(NULL, 0, NULL);//главный и единственный мьютекс
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
                    voditeliChisloC[curDigit] = key[0];
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
    hwnd = CreateWindow("MyClass", "OurWindow", WS_OVERLAPPEDWINDOW, 0, 0, 800, 800, 0, 0, hInstance, NULL);
    hdc = GetDC(hwnd);
    ShowWindow(hwnd, nCmdShow);//тут заканчивается создание окна

    brushGreen = CreateSolidBrush(RGB(64,128, 32));//создаем кисточки
    brushBlue = CreateSolidBrush(RGB(32,128,255));
    brushGray = CreateSolidBrush(RGB(128,128,128));
    brushWhite = CreateSolidBrush(RGB(200,200,200));
    pen = CreatePen(PS_SOLID, 1, RGB(0,0,0)); 

    


    char text1[] = "Vvedite chislo voditelov";//ввод числа человечков
    TextOut(hdc, 32, 32, text1, sizeof(text1));

    while(GetMessage(&msg, 0, 0, 0) && curDigit > -1)
    {
        DispatchMessage(&msg);
    }

    sscanf(voditeliChisloC, "%d", &voditeliChislo);//преобразование полученной строки в цифру



    for(int i = 0; i < voditeliChislo; i++)//создание потоков с человечками
    {
        voditel vod1;//создаем пассажира, настраиваем его параметры
        voditeli[i] = vod1;//ложим его  в масссива
        HANDLE hthr;
        hthr = (HANDLE)_beginthread(voditelFunction, 0,(void*)i);//непосредественнно запуск потока и передаем номер ячейки (i) массива с этим пассажиром
    }

    while(1)//в данном цикле описан алгоритм работы самолета
    {
        if(PeekMessage(&msg, hwnd,  0, 0, PM_REMOVE))//мониторим события окна
        {
            DispatchMessage(&msg);
        }

        paintFunction();//рисуем все

        Sleep(1000/tScorost);//задержка между кадрами
    }
}