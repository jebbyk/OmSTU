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
char passChisloC[4] = "";//строка куда вводится число пассажиров
int passChislo = 1;//число в котором преобразуется число пасиков из строки в цифру
int curDigit = 0;//для вода числа пасиков тукущий вводимый разряд числа)

HBRUSH brushGreen;//кисти для заливки разными цветами
HBRUSH brushGray;
HBRUSH brushBlue;
HBRUSH brushBlack;
HBRUSH brushLightBlue;
HBRUSH brushWhite;

HPEN pen;//карандаш для рисования линий

POINT samoletForma[] = {//координаты точек по которым строится форма самолета
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
POINT samoletPosRis;//позиция самолета для рисования
float samoletPosX;//уточненная позиция самолета(нужно для нормлаьного перемещения)
float samoletPosY;

POINT samoletReis;//позиция куда самолету надо лететь
BOOL samoletStop;//стоит ли самолет на стоянке
BOOL samoletPolet;//или же летит
int samoletPass;//число пасиков в самолете
int samoletGorodTekush = 0;//текущий город в котром находится самолет
int samoletSon = 0;//сон самолета(для ожидания)
float samoletScorost = 0;//скорость самолета

int tScorost  = 30;//скорость и плавность анимации

char text[12] = "";


POINT gorod1Pos = { 128, 128};//позиции городов 
POINT gorod2Pos = { 620, 200};
POINT gorod3Pos = { 500, 600};
POINT gorod4Pos = { 200, 580};

POINT oblast1Forma[] = {//координаты с формами для зеленых и серых областей
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



HRGN oblast1;//переменные которые буду хранить неподсредственно фигуру областей
HRGN gorod1;
HRGN oblast2;
HRGN gorod2;
HRGN oblast3;
HRGN gorod3;
HRGN oblast4;
HRGN gorod4;
    
HANDLE hmtx;



typedef struct {//структура с данными об отдельном пассажире
    float fx;//точные координаты (тоже для перемещения)
    float fy;
    int x;//целочисленные координаты для риосвания
    int y;
    int cx;
    int cy;
    BOOL walkAeroport;//к аеропорту
    BOOL walkGorod;//к гоороду
    BOOL samolet;//в самолете
    BOOL ojidanie;// ждет самолета
    BOOL son;//спит в городе
    int sonDlinna;//число сколько еще спать
    int gorodTekush;//городо в котором пассажир находится
}passajir;

passajir passajiry[1024];//массив с пассажирами

void passajirFunction(void *arg)//функция пассажира (с ней тред запускается)
{
    srand(arg);
    int i = (int)arg;//передаем аргумент, по которому определяем какой именно элемент массива модифицеровать
    passajiry[i].cx = gorod1Pos.x;//указываем начальные целевые координаты
    passajiry[i].cy = gorod1Pos.y;
    passajiry[i].gorodTekush = 0;//город
    passajiry[i].walkAeroport = TRUE;//и начальное состояние, что он идет в аеропорт


    while(TRUE)
    {
        if(passajiry[i].walkAeroport == TRUE)//если пассажир сейчас идет в аеропрт
        {
            float dx;
            float dy;
            dx = passajiry[i].cx - passajiry[i].fx;//берем вектор по направлению к целевой точке
            dy = passajiry[i].cy - passajiry[i].fy;
            
            float l = sqrt(dx*dx + dy*dy);// узнаем рассатояние
            if(l > 1.0f)
            {
                dx /= l;//нормализуем вектор
                dy /= l;
            }

            passajiry[i].fx += dx;//перемещаем человека
            passajiry[i].fy += dy;
            passajiry[i].x = passajiry[i].fx;
            passajiry[i].y = passajiry[i].fy;

            if(passajiry[i].fx > passajiry[i].cx -1 && passajiry[i].fx < passajiry[i].cx + 1 &&        passajiry[i].fy > passajiry[i].cy -1 && passajiry[i].fy < passajiry[i].cy + 1)
            {
                passajiry[i].walkAeroport = FALSE;//меняем его состояние на ожидание, если он очень близко к целевой точке
                passajiry[i].ojidanie = TRUE;
            }

        }
        if(passajiry[i].ojidanie == TRUE)//если сейчас ожидание
        {
            float dx;
            float dy;
            dx = samoletPosX - passajiry[i].fx;
            dy = samoletPosY - passajiry[i].fy;
            float l = sqrt(dx*dx + dy*dy);//так же беерм вектор до самолета и узнаем расстояние
            WaitForSingleObject(hmtx, INFINITE);
            if(l < 4.0f && samoletPass < 4 && samoletStop == TRUE)//если оно очень маленькое то пытаемся сесть в самолет обязательно блокируя всем остальным людям доступ к самолету
            {
                
                passajiry[i].ojidanie = FALSE;
                passajiry[i].samolet = TRUE;
                samoletPass++;
                passajiry[i].sonDlinna = 200;//счетчик чтоб человек не спрыгивал с самолета когда не надо
                
            }
            ReleaseMutex(hmtx);//потом отпускаем мьютекс
        }
        if(passajiry[i].samolet == TRUE)//если пассажир в самолете
        {
            passajiry[i].sonDlinna--;
            passajiry[i].fx = samoletPosX;//перемещаем человека вместе с самолетом
            passajiry[i].fy = samoletPosY;
            passajiry[i].x = passajiry[i].fx;
            passajiry[i].y = passajiry[i].fy;
            if(samoletStop == TRUE)//если самолет остановился
            {
                if(passajiry[i].sonDlinna <=0)// и уже можно слазить
                {
                    passajiry[i].samolet = FALSE;//слазием с самолета
                    passajiry[i].gorodTekush = samoletGorodTekush;
                    passajiry[i].walkGorod = TRUE;
                    
                   
                    if(passajiry[i].gorodTekush == 0)//берем рандомную позицию по координатам города
                    {
                        passajiry[i].cx = (rand() % 100) + gorod1Pos.x - 50;
                        passajiry[i].cy = (rand() % 100) + gorod1Pos.y - 50;
                    }
                    if(passajiry[i].gorodTekush == 1)
                    {
                        passajiry[i].cx = (rand() % 100) + gorod2Pos.x - 50;
                        passajiry[i].cy = (rand() % 100) + gorod2Pos.y - 50;
                    }
                    if(passajiry[i].gorodTekush == 2)
                    {
                        passajiry[i].cx = (rand() % 100) + gorod3Pos.x - 50;
                        passajiry[i].cy = (rand() % 100) + gorod3Pos.y - 50;
                    }
                    if(passajiry[i].gorodTekush == 3)
                    {
                        passajiry[i].cx = (rand() % 100) + gorod4Pos.x - 50;
                        passajiry[i].cy = (rand() % 100) + gorod4Pos.y - 50;
                    }
                }
            }
        }
        if(passajiry[i].walkGorod == TRUE)//если идем в город
        {
            float dx;
            float dy;
            dx = passajiry[i].cx - passajiry[i].fx;//такое же перемещение по вектору до точки
            dy = passajiry[i].cy - passajiry[i].fy;
            
            float l = sqrt(dx*dx + dy*dy);
            if(l > 1.0f)
            {
                dx /= l;
                dy /= l;
            }

            passajiry[i].fx += dx;
            passajiry[i].fy += dy;
            passajiry[i].x = passajiry[i].fx;
            passajiry[i].y = passajiry[i].fy;

            if(passajiry[i].fx > passajiry[i].cx -1 && passajiry[i].fx < passajiry[i].cx + 1 &&        passajiry[i].fy > passajiry[i].cy -1 && passajiry[i].fy < passajiry[i].cy + 1)
            {
                passajiry[i].walkGorod = FALSE;//и если дошли, то засыпаем на долго
                passajiry[i].son = TRUE;
                passajiry[i].sonDlinna = rand() % 30000 + 30000;
                Sleep(passajiry[i].sonDlinna);
                passajiry[i].son = FALSE;
                passajiry[i].walkAeroport = TRUE;//после того как проснулись, снова идем к аеропроту
                
                if(passajiry[i].gorodTekush == 0)
                {
                    passajiry[i].cx = gorod1Pos.x;
                    passajiry[i].cy = gorod1Pos.y;
                }
                if(passajiry[i].gorodTekush == 1)
                {
                    passajiry[i].cx = gorod2Pos.x;
                    passajiry[i].cy = gorod2Pos.y;
                }
                if(passajiry[i].gorodTekush == 2)
                {
                    passajiry[i].cx = gorod3Pos.x;
                    passajiry[i].cy = gorod3Pos.y;
                }
                if(passajiry[i].gorodTekush == 3)
                {
                    passajiry[i].cx = gorod4Pos.x;
                    passajiry[i].cy = gorod4Pos.y;
                }
            }
        }
      
        Sleep(1000/tScorost);//задержка между отедльными кадрами анимации
    }
}


void paintFunction()//фнукция рисования
{
    Rectangle(hdc, 0,0,800,800);//монотонная заливка всего экрана

    
    FillRgn(hdc, oblast1, brushGreen);//отрисоовка городов
    FillRgn(hdc, gorod1, brushGray);

    FillRgn(hdc, oblast2, brushGreen);
    FillRgn(hdc, gorod2, brushGray);

    FillRgn(hdc, oblast3, brushGreen);
    FillRgn(hdc, gorod3, brushGray);

    FillRgn(hdc, oblast4, brushGreen);
    FillRgn(hdc, gorod4, brushGray);

    POINT _samoletForma[sizeof(samoletForma)/8];//берем форму самолета
    
    for(int i = 0; i < sizeof(_samoletForma)/8; i++)//потом перемещаем ее в нужную позицию
    {
        _samoletForma[i].x = samoletForma[i].x + samoletPosRis.x;
        _samoletForma[i].y = samoletForma[i].y + samoletPosRis.y;
    }
    HRGN samolet = CreatePolygonRgn(_samoletForma, sizeof(samoletForma)/8,WINDING );//и заливаем
    FillRgn(hdc, samolet, brushWhite);

    
    for(int i = 0; i < passChislo; i++)
    {
        Rectangle(hdc, passajiry[i].x-1, passajiry[i].y-1, passajiry[i].x+1, passajiry[i].y+1);//рисуем маленьких человечков
    }
}





void main()
{
    hmtx = CreateMutex(NULL, FALSE, NULL);//главный и единственный мьютекс
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
    hwnd = CreateWindow("MyClass", "OurWindow", WS_OVERLAPPEDWINDOW, 0, 0, 800, 800, 0, 0, hInstance, NULL);
    hdc = GetDC(hwnd);
    ShowWindow(hwnd, nCmdShow);//тут заканчивается создание окна

    brushGreen = CreateSolidBrush(RGB(64,128, 32));//создаем кисточки
    brushBlue = CreateSolidBrush(RGB(32,128,255));
    brushGray = CreateSolidBrush(RGB(128,128,128));
    brushWhite = CreateSolidBrush(RGB(200,200,200));
    pen = CreatePen(PS_SOLID, 1, RGB(0,0,0)); 

    


    char text1[] = "Vvedite chislo passajirov";//ввод числа человечков
    TextOut(hdc, 32, 32, text1, sizeof(text1));

    while(GetMessage(&msg, 0, 0, 0) && curDigit > -1)
    {
        DispatchMessage(&msg);
    }

    sscanf(passChisloC, "%d", &passChislo);//преобразование полученной строки в цифру


  
    for(int i = 0; i < sizeof(oblast1Forma)/8; i++)//расположение городов в нужных позициях
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

    oblast1 = CreatePolygonRgn(oblast1Forma, sizeof(oblast1Forma)/8, WINDING);//создания самих фигур для них
    gorod1 = CreatePolygonRgn(gorod1Forma, sizeof(gorod1Forma)/8, WINDING);
    oblast2 = CreatePolygonRgn(oblast2Forma, sizeof(oblast2Forma)/8, WINDING);
    gorod2 = CreatePolygonRgn(gorod2Forma, sizeof(gorod2Forma)/8, WINDING);
    oblast3 = CreatePolygonRgn(oblast3Forma, sizeof(oblast3Forma)/8, WINDING);
    gorod3 = CreatePolygonRgn(gorod3Forma, sizeof(gorod3Forma)/8, WINDING);
    oblast4 = CreatePolygonRgn(oblast4Forma, sizeof(oblast4Forma)/8, WINDING);
    gorod4 = CreatePolygonRgn(gorod4Forma, sizeof(gorod4Forma)/8, WINDING);



    for(int i = 0; i < passChislo; i++)//создание потоков с человечками
    {
        passajir pas1;//создаем пассажира, настраиваем его параметры
        pas1.fx = rand() % 100 + gorod1Pos.x - 50;
        pas1.fy = rand() % 100 + gorod1Pos.y - 50;
        pas1.x = pas1.fx;
        pas1.y = pas1.fy;
        pas1.sonDlinna = rand()%30000 + 30000;
        passajiry[i] = pas1;//ложим его  в масссива
        HANDLE hthr;
        hthr = (HANDLE)_beginthread(passajirFunction, 0,(void*)i);//непосредественнно запуск потока и передаем номер ячейки (i) массива с этим пассажиром
    }
    
    samoletReis.x = gorod4Pos.x;
    samoletReis.y = gorod4Pos.y;
    samoletPolet = TRUE;
    samoletGorodTekush = 3;//создание первоначального самолета с его состоянием

    while(TRUE)//в данном цикле описан алгоритм работы самолета
    {
        if(samoletPolet == TRUE)
        {
            float dx;
            float dy;
            dx = (double)samoletReis.x - samoletPosX;
            dy = (double)samoletReis.y - samoletPosY;
            
            float l = sqrt(dx*dx + dy*dy);
            dx /= l;
            dy /= l;//перемещение по вектору

            if(samoletScorost < l/10)
            {
                samoletScorost++;//ускорение до опр предела
            }else{
                samoletScorost = l/10;
            }
            if(samoletScorost > 10)
            {
                samoletScorost = 10;//торможение если близко к цели
            }
            


            samoletPosX += dx*samoletScorost;
            samoletPosY += dy*samoletScorost;
            samoletPosRis.x = samoletPosX;
            samoletPosRis.y = samoletPosY;

            if(samoletPosX > samoletReis.x - 1 && samoletPosX < samoletReis.x + 1 &&     samoletPosY > samoletReis.y - 1 && samoletPosY < samoletReis.y + 1)    
            {
                samoletPolet = FALSE;//елси очень очень близко к цели, то самолет на идет на стоянку
                samoletPass = 0;
                samoletSon = 60;//ждет
                samoletStop = TRUE;
            }
        }

        if(samoletSon > 0)//пока самолет на стоянке
        {
            samoletSon--;
        }else{
            if(samoletGorodTekush == 0 && samoletPolet == FALSE) //елси уже закончилось время ожидания
            {
                samoletGorodTekush = 1;//выбирает следующий город
                samoletReis.x = gorod2Pos.x;//координаты
                samoletReis.y = gorod2Pos.y;
                samoletPolet = TRUE;//и летит
                samoletStop = FALSE;
            }
            if(samoletGorodTekush == 1 && samoletPolet == FALSE) 
            {
                samoletGorodTekush = 2;
                samoletReis.x = gorod3Pos.x;
                samoletReis.y = gorod3Pos.y;
                samoletPolet = TRUE;
                samoletStop = FALSE;
            }
            if(samoletGorodTekush == 2 && samoletPolet == FALSE) 
            {
                samoletGorodTekush = 3;
                samoletReis.x = gorod4Pos.x;
                samoletReis.y = gorod4Pos.y;
                samoletPolet = TRUE;
                samoletStop = FALSE;
            }
            if(samoletGorodTekush == 3 && samoletPolet == FALSE)
            {
                samoletGorodTekush = 0;
                samoletReis.x = gorod1Pos.x;
                samoletReis.y = gorod1Pos.y;
                samoletPolet = TRUE;
                samoletStop = FALSE;
            }   
        }

        if(PeekMessage(&msg, hwnd,  0, 0, PM_REMOVE))//мониторим события окна
        {
            DispatchMessage(&msg);
        }

        paintFunction();//рисуем все

        Sleep(1000/tScorost);//задержка между кадрами
    }
}