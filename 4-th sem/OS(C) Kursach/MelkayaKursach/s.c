#include <stdio.h>
#include <Windows.h>
#include <math.h>//библиотека для математических функций

HPEN pen;

HANDLE hstdout, hmtx;//хендл стандартного вывода

HINSTANCE hInstance;//для создания окна
STARTUPINFO si;//информация о старте окна
int nCmdShow;
HWND hwnd;//дескриптор окна
HDC hdc;//дескриптор для рисования
MSG msg;//сообщение передаваемое окном
WNDCLASS wc;//для создания окна
char people_countC[4] = "";//строка куда вводится число пассажиров
int people_count = 1;//число в котором преобразуется число пасиков из строки в цифру
int curDigit = 0;//для вода числа пасиков тукущий вводимый разряд числа)


unsigned short target_framerate = 15;//плавность и скорость работы

unsigned int level_size  = 32;//размер этажа

typedef struct{//структура для координат
    float x;
    float y;
}coord;


typedef struct {//структура для описсания этажа
    coord position;//координаты этажа
    coord stop;//координаты остановочного места для одного лифта
}level;

level levels_list[6];

void Setlevels()//расположение всех этажей и остановочных мест для каждого этажа
{
    levels_list[0].position.x = 512;//координаты самих этажей
    levels_list[0].position.y = 96 * 6;

    levels_list[1].position.x = 512;
    levels_list[1].position.y = 96 * 5;

    levels_list[2].position.x = 512;
    levels_list[2].position.y = 96 * 4;

    levels_list[3].position.x = 512;
    levels_list[3].position.y = 96 * 3;

    levels_list[4].position.x = 512;
    levels_list[4].position.y = 96 * 2;

    levels_list[5].position.x = 512;
    levels_list[5].position.y = 96 * 1;

    for(int i = 0; i < 6; i++)
    {
        levels_list[i].stop.x = levels_list[i].position.x -64;//координаты остановочных мест для каждого этажа (для движения в двух лифтах)
        levels_list[i].stop.y = levels_list[i].position.y ;
    }   
}


typedef struct{//структура описывающая персонажей
    coord position;//текущий позиция персонажа
    coord target_position;//кординаты точки, куда нужно идти человеку
    short drive;//флаг говорящий о том что человвек едет
    short sleep;//спит
    short walk_level;//идет к остановке
    short walk_elev;//идет к лифту
    short walk_out;//идет от этажа
    short wait;//ждет
    short direction;//направление в котором хочет ехать человек
    short current_level;//текущий этаж, на котором находится человек
    short current_elev;
    float speed;//текущий скорость перемещения человека
}people;

typedef struct{//структура описывающая лифт
    coord position;//текущий позиция
    coord target_position;//точка в которую едет лифт
    float speed;//его скорость
    short drive;//флаг, который указывает, едет ли сейчас лифт
    short wait;//ждет на этаже
    short people_amount;//количество лудй внутри
    short current_level;//текущий этаж, на котором находится лифт
    short direction;//направление движения лифту
}elev;

elev elev1;//сам лифт
elev elev_list[2];

people people_list[1024];//массив содержащий множество структур со всеми людьми


void peopleThreadFunction(void *_i)//функция с алгоритмом поведения персонажей
{
    
    int i = _i;//сохраняемый переданный функции аргумент, это будет номер элемента массива с людьми, к оторому у функции будте доступ
    srand(i);

    people_list[i].current_level = rand()%6;//задаем рандомную остановку при старрте

    people_list[i].direction = rand()%2;//задаем ранодомное направление
    people_list[i].current_elev = 0;
    if(people_list[i].current_level == 0) people_list[i].direction = 0;//если этаж - нижний, то направление - вперед
    if(people_list[i].current_level == 5) people_list[i].direction = 1;//если этаж - верхний, то направление - обратное
    
    people_list[i].target_position = levels_list[people_list[i].current_level].stop;//указываем точку движения персонажа соответствующие  


    people_list[i].target_position.x += -8 + rand()%16;//добавляем рандомный разброс, чтоб толпились возле лифта
    people_list[i].target_position.y += -8 + rand()%16;

    people_list[i].position = levels_list[people_list[i].current_level].position;//указываем начальные координаты самих персонажей
    people_list[i].position.x += -128 + rand()%256;
    people_list[i].position.y += -16 + rand()%32;

    people_list[i].walk_level = 1;//ставим флаг, что персоонаж движется к лифту

    while(1)//бесконечный цикл для непрерыной обработки персонажей
    {
    
        if(people_list[i].walk_level)//елси текущий флаг - движение к лифту
        {
            coord dir_vector = {people_list[i].target_position.x-people_list[i].position.x, people_list[i].target_position.y-people_list[i].position.y};//рассчитываем вектор для движеня персонажа
            float l = sqrt(pow(dir_vector.x,2)+pow(dir_vector.y, 2));//рассчитываем длину этого вектора
            coord normal_v  = dir_vector;//нормализованный вектор, который должен иметь то же направление, но длинну 1
            normal_v.x /= l;//для этого делим его на длину первого вектора
            normal_v.y /= l;
            if(people_list[i].speed < 0.5 && people_list[i].speed < l/20)//если скорость персонажа меньше 0.5 и в принципе достаточно млаа
            {
                people_list[i].speed += 0.01;//то постеменно ее увеличиваем
            }
            if(people_list[i].speed > l/20)//если скорость слишком велика по отношению к оставшемуся расстоянию до цели, 
            {
                people_list[i].speed = l/20;//то снижаем ее
            }

            people_list[i].position.x += normal_v.x * people_list[i].speed;//изменяем координаты персонажа на основании его скорости и нормализованного вектора длвижения
            people_list[i].position.y += normal_v.y * people_list[i].speed;


            if(people_list[i].speed < 0.001)//если персонаж остановился
            {
                people_list[i].walk_level = 0;//сбрасываем флаг, что он движется к остановке
                people_list[i].wait = 1;//устанавливаем флаг ожидания транспорта
            }
        }

        if(people_list[i].wait > 0)//если активен флаг ожидания
        {
            //смотрим, не переполнен ли лифт, остановился ли он, и находится ли лифт на той же остановке, что и персонаж
            if(people_list[i].direction == elev_list[people_list[i].current_elev].direction && people_list[i].current_level == elev_list[people_list[i].current_elev].current_level && elev_list[people_list[i].current_elev].people_amount < 3)
            {
                people_list[i].walk_elev = 1;//елси это так, то устанавливаем флаг, что персонаж движется к лифту
                people_list[i].wait = 0;//сбрасываем флаг ожидания транспорта
                people_list[i].target_position = elev_list[people_list[i].current_elev].position;//утсанавливаем точку перемещения персонжа - координаты лифта
            }
        }

        if(people_list[i].walk_elev == 1)//если активен флаг передвижения к лифту
        {
            if(elev_list[people_list[i].current_elev].people_amount < 4 && elev_list[people_list[i].current_elev].wait > 0)//если лифт все еще ждет и там есть свободные места
            {
                coord dir_vector = {people_list[i].target_position.x-people_list[i].position.x, people_list[i].target_position.y-people_list[i].position.y};//активируем алгоритм перемещения
                float l = sqrt(pow(dir_vector.x,2)+pow(dir_vector.y, 2));
                coord normal_v  = dir_vector;
                normal_v.x /= l;
                normal_v.y /= l;
                if(people_list[i].speed < 0.5 && people_list[i].speed < l/20)
                {
                    people_list[i].speed += 0.01;
                }
                if(people_list[i].speed > l/20)
                {
                    people_list[i].speed = l/20;
                }

                people_list[i].position.x += normal_v.x * people_list[i].speed;
                people_list[i].position.y += normal_v.y * people_list[i].speed;


                if(people_list[i].speed < 0.01)//и если человек почти дошел
                {
                    WaitForSingleObject(hmtx, INFINITE);//захватываем мьютексб на доступ к лифту
                    people_list[i].drive = 1;//устанавливаем флаг езды
                    elev_list[people_list[i].current_elev].people_amount++;//увеличиваем у лифта информацию о количестве пассажирова
                    people_list[i].walk_elev = 0;//сбрасываем флаг движения к лифту
                    ReleaseMutex(hmtx);//отпускаем мьютекс
                }
            }else{//елси лифт уже поехал, или там уже закончились места
                people_list[i].walk_level = 1;//идем обратно на туже остановку
                people_list[i].walk_elev = 0;//сбарсываем флаг движения к лифту
                people_list[i].target_position = levels_list[people_list[i].current_level].stop;//выбираем координаты нужного остановочного места как цлеевые/
                people_list[i].target_position.x += -8 + rand()%16;//добавляем немного рандома для создания толпы
                people_list[i].target_position.y += -8 + rand()%16;
            }
        }
        if(people_list[i].drive == 1)//есди установлен флаг движени в лифту
        {
            people_list[i].position = elev_list[people_list[i].current_elev].position;//приравнивем координаты пассажира к координатам лифта
            if(elev_list[people_list[i].current_elev].wait && people_list[i].current_level != elev_list[people_list[i].current_elev].current_level)//еслли лифт остановился, и этаж не равна предыдущей остановке, на котором былл человек
            {
                people_list[i].current_level = elev_list[people_list[i].current_elev].current_level;//обновляем информацию о текущей остановке персонажа
                int r = rand()%2;//делаем рандом который вернет 1 или 0
                if(r == 1)//елс 1 то 
                {
                    WaitForSingleObject(hmtx, INFINITE);//захватываем мьютекс
                    people_list[i].drive = 0;//сбрасываем флаг езда на лифту
                    people_list[i].walk_out = 1;//устанавливаем флаг движения от этажа
                    elev_list[people_list[i].current_elev].people_amount--;//уменьшаем информациию о количестве пассажиров в  лифте
                    people_list[i].target_position = levels_list[people_list[i].current_level].position;//указываем целевую точку движения - координаты текущего этажа
                    people_list[i].target_position.x += -64 + rand()%128;//и добавляем большой разброс в координтаах
                    if(people_list[i].current_level == 0) people_list[i].target_position.x += -512 + rand()%1024;
                    people_list[i].target_position.y += -16 + rand()%32;
                    ReleaseMutex(hmtx);//отпускаем мьютекс
                }
            }
        }
        if(people_list[i].walk_out == 1)//если установлен флаг движения в направлении  от становки
        {
            coord dir_vector = {people_list[i].target_position.x-people_list[i].position.x, people_list[i].target_position.y-people_list[i].position.y};//запускается алгорим перемещения
            float l = sqrt(pow(dir_vector.x,2)+pow(dir_vector.y, 2));
            coord normal_v  = dir_vector;
            normal_v.x /= l;
            normal_v.y /= l;
            if(people_list[i].speed < 0.5 && people_list[i].speed < l/20)
            {
                people_list[i].speed += 0.01;
            }
            if(people_list[i].speed > l/20)
            {
                people_list[i].speed = l/20;
            }

            people_list[i].position.x += normal_v.x * people_list[i].speed;
            people_list[i].position.y += normal_v.y * people_list[i].speed;


            if(people_list[i].speed < 0.01)//елси дошел до цлеи
            {
                people_list[i].walk_out = 0;//сбрасыаем флаг движения  от этажа
                people_list[i].sleep = 1;//устанавливаем флаг сна
                Sleep(10*(rand()%100 + 100));//засыпаем на неопределенное количество времени
                people_list[i].sleep = 0;//после сна сбрасываем флаг сна
                people_list[i].walk_level = 1;//устанавливаем флаг движения к остановке снова
                
                people_list[i].direction = rand()%2;//делаем рандомное направление в котором хочет ехть человек
                people_list[i].current_elev = 0;
                if(people_list[i].current_level == 0) people_list[i].direction = 0;//елси текущий этаж - крайняя  то точно указываем направление в котором вообще можно ехать
                if(people_list[i].current_level == 5) people_list[i].direction = 1;
                

                people_list[i].target_position = levels_list[people_list[i].current_level].stop;//выбираем нужное остановочное место
                people_list[i].target_position.x += -8 + rand()%16;//и добавляем рандом для создания толпы
                people_list[i].target_position.y += -8 + rand()%16;
                
            }
        }
        Sleep(100/target_framerate);//отправляем поток в сон на 1 шестидесятую секунды.
    }
}


void showContent()//фнукция для отображения всего происходящего через окно
{
    Rectangle(hdc, 0,0,1000,800);
    
    coord op;//вспомогательная координата предыдущей этажа для рисования дороги 
    for(int i = 0; i < 6; i++)//для всех четырех остановок
    {
        coord p = levels_list[i].position;//вспомогательная координата текущей этажа
        coord p1 = levels_list[i].stop;//и выбираем координаты нужного остановочного метса

        Rectangle(hdc,   p.x-256, p1.y-level_size/2,  p.x+256, p1.y+level_size/2);
        
        Rectangle(hdc,  p1.x-level_size/2, p1.y-level_size/2,  p1.x+level_size/2, p1.y+level_size/2);

    }

    Rectangle(hdc,  elev_list[0].position.x-8, elev_list[0].position.y - 16,  elev_list[0].position.x+8, elev_list[0].position.y + 16);

    
    char pc[4] = "";
    sprintf(pc, "%d", people_count);//преобразуем количество людей в строку

    for(int i = 0; i < people_count; i++)//проходим циклом по всему ссписку людей в масиве
    {
        if(people_list[i].sleep != 1) Rectangle(hdc,    people_list[i].position.x - 2, people_list[i].position.y - 2, people_list[i].position.x + 2, people_list[i].position.y + 2);
    }
}


void ElevThreadFunction(void *arg)
{
    while(1)//запускаем безконечный цикл
    {
        for(int j = 0; j < 2; j++)
        {
            if(elev_list[j].drive == 1)//в нем проверяем флаг езды, и еси он 1, то
            {
                coord dir_vector = {elev_list[j].target_position.x-elev_list[j].position.x, elev_list[j].target_position.y-elev_list[j].position.y};//выполняем перемещение так же как и у пассажиров, только с более высокой скоростью
                float l = sqrt(pow(dir_vector.x,2)+pow(dir_vector.y, 2));
                coord normal_v  = dir_vector;
                normal_v.x /= l;
                normal_v.y /= l;
                if(elev_list[j].speed < 10 && elev_list[j].speed < l/20)//сдесь ограничение скорости выставленно в 2 раза выше чем у человеков
                {
                    elev_list[j].speed += 0.05;
                }
                if(elev_list[j].speed > l/20)
                {
                    elev_list[j].speed = l/20;
                }

                elev_list[j].position.x += normal_v.x * elev_list[j].speed;
                elev_list[j].position.y += normal_v.y * elev_list[j].speed;


                if(elev_list[j].speed < 0.001)//елси лифту почти полностью осттановилось, то 
                {
                    elev_list[j].drive = 0;//останавливаем его полностью, сбрасывае флаг езды
                    elev_list[j].wait = 360;//устанавливая вемя ожиданя
                    if(elev_list[j].direction == 0)//дальше проверяем, направление движени
                    {
                        elev_list[j].current_level++;//прибавляем 1 к информации о ткущей остановке лифту
                        if(elev_list[j].current_level == 5) //еси она оказалась на крайней остановке
                        {
                            elev_list[j].direction = 1;//изменяем направление движения на обратное
                        }
                    }else{//тут вспе тоже самое для только для обратного направления движения
                        elev_list[j].current_level--;
                        if(elev_list[j].current_level == 0) 
                        {  
                            elev_list[j].direction = 0;
                        }
                    }      
                }
            }
            if(elev_list[j].wait > 0)//елси еще не закончислоь время ожидания
            {
                elev_list[j].wait--;//то ждем,и немного это времяуменьшаем
                if(elev_list[j].wait == 0) //елси время истекло
                { 
                    elev_list[j].drive = 1;//устанавливаем флаг езды
                    int c;
                    if(elev_list[j].direction == 0)//в зависимости от направления движения
                    {
                        c = elev_list[j].current_level + 1;////выбираем слудующую останвоку
                    }
                    if(elev_list[j].direction == 1)
                    {
                        c = elev_list[j].current_level - 1;
                    }
                    elev_list[j].target_position.y = levels_list[c].position.y;//передаем ее координаты в целевую точку для движения
                   if(j == 0) elev_list[j].target_position.x = levels_list[c].position.x ;
                   else elev_list[j].target_position.x = levels_list[c].position.x ;
                }
            }
        }
        Sleep(100/target_framerate);//отправляем основной поток вв сон на одну шестидесятую секунды
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
                    people_countC[curDigit] = key[0];
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
    hwnd = CreateWindow("MyClass", "OurWindow", WS_OVERLAPPEDWINDOW, 0, 0, 1000, 800, 0, 0, hInstance, NULL);
    hdc = GetDC(hwnd);
    ShowWindow(hwnd, nCmdShow);//тут заканчивается создание окна

   
    pen = CreatePen(PS_SOLID, 1, RGB(0,0,0)); 

    


    char text1[] = "Vvedite chislo passajirov";//ввод числа человечков
    TextOut(hdc, 32, 32, text1, sizeof(text1));

    while(GetMessage(&msg, 0, 0, 0) && curDigit > -1)
    {
        DispatchMessage(&msg);
    }

    sscanf(people_countC, "%d", &people_count);

    Setlevels();//выполняем расположение остановк и остановочных мест в соответствии с координатами

    for(int i = 0; i < people_count; i++)//запускаем цикл на количество итераций равному числу, которое ввел польователь
    {
        people p;//создаем переменную для человека
        HANDLE hthr;
        hthr = (HANDLE)_beginthread(peopleThreadFunction, 0,(void*)i);
    }

    HANDLE hthr;
    hthr = (HANDLE)_beginthread(ElevThreadFunction, 0,(void*)0);


     
    //дальше идет алгоритм для маршрутного лифту
    elev1.drive = 1;//усатнавливаем, что при старте программы лифту едет
    elev1.direction = 1;//указываем направление (на первой же остановке оно поменяется)
    elev1.current_level = 1;//и сотановка
    elev1.target_position.y = levels_list[0].position.y;//укаываем начальную координату
    elev1.target_position.x = levels_list[0].position.x;
    elev1.position.y = levels_list[0].position.y+1;//укаываем начальную координату
    elev1.position.x = levels_list[0].position.x;

    elev_list[0] = elev1;
    
    while(1)
    {
        if(PeekMessage(&msg, hwnd,  0, 0, PM_REMOVE))//мониторим события окна
        {
            DispatchMessage(&msg);
        }
        showContent();//выполняем отображене всех людей, лифту и остановоок в графическом окне
        Sleep(1000/target_framerate);//отправляем основной поток вв сон на одну шестидесятую секунды
    }
}