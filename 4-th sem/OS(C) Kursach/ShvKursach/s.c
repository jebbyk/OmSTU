#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <X11/Xlib.h>//библиотека для графики
#include <math.h>//библиотека для математических функций

unsigned short target_framerate = 60;//плавность и скорость работы
Display *dspl;//ссылка на дисплей для рисования
int screen;
Window hwnd;//ссылка на окно в котором происходит рисование
XEvent event;//события окна
KeySym key_sym;//переменная для ковертирования клавиш в цифрыы 
GC gc;//ссылк на обьект для рисования
pthread_mutex_t hmtx;//мьютекс для блокировки доступа к лифту

XColor SvZelen, tZeleny, white, gray, sery, jolty, krasny, siny;//используемые цвета
unsigned int level_size  = 32;//размер этажа

typedef struct{//структура для координат
    float x;
    float y;
}coord;


typedef struct {//структура для описсания этажа
    coord position;//координаты этажа
    coord level_first;//координаты остановочного места для одного лифта
    coord level_second;//координаты остановочного места для второго
}level;

level etajMassiv[6];

void Setlevels()//расположение всех этажей и остановочных мест для каждого этажа
{
    etajMassiv[0].position.x = 390;//координаты самих этажей
    etajMassiv[0].position.y = 96 * 6;

    etajMassiv[1].position.x = 390;
    etajMassiv[1].position.y = 96 * 5;

    etajMassiv[2].position.x = 390;
    etajMassiv[2].position.y = 96 * 4;

    etajMassiv[3].position.x = 390;
    etajMassiv[3].position.y = 96 * 3;

    etajMassiv[4].position.x = 390;
    etajMassiv[4].position.y = 96 * 2;

    etajMassiv[5].position.x = 390;
    etajMassiv[5].position.y = 96 * 1;

    for(int i = 0; i < 6; i++)
    {
        etajMassiv[i].level_first.x = etajMassiv[i].position.x - 128;//координаты остановочных мест для каждого этажа (для движения в двух лифтах)
        etajMassiv[i].level_first.y = etajMassiv[i].position.y ;

        etajMassiv[i].level_second.x = etajMassiv[i].position.x  + 128;
        etajMassiv[i].level_second.y = etajMassiv[i].position.y;
    }   
}


typedef struct{//структура описывающая персонажей
    coord position;//текущий позиция персонажа
    coord MoveTo;//кординаты точки, куда нужно идти человеку
    short drive;//флаг говорящий о том что человвек едет
    short sleep;//спит
    short walk;//идет к остановке
    short vLift;//идет к лифту
    short fromLift;//идет от этажа
    short wait;//ждет
    short direction;//направление в котором хочет ехать человек
    short etaj;//текущий этаж, на котором находится человек
    short current_elev;
    float speed;//текущий скорость перемещения человека
    float max_speed;
}people;

typedef struct{//структура описывающая лифт
    coord position;//текущий позиция
    coord MoveTo;//точка в которую едет лифт
    float speed;//его скорость
    short drive;//флаг, который указывает, едет ли сейчас лифт
    short wait;//ждет на этаже
    short chelCount;//количество лудй внутри
    short etaj;//текущий этаж, на котором находится лифт
    short direction;//направление движения лифту
}elev;

elev elev1;//сам лифт
elev elev2;
elev liftMassiv[2];

people chelovekMassiv[1024];//массив содержащий множество структур со всеми людьми
short N;//переменная хранящая количесттво людей котрое ввел пользователь


void chleThread(void *_i)//функция с алгоритмом поведения персонажей
{
    int i = _i;//сохраняемый переданный функции аргумент, это будет номер элемента массива с людьми, к оторому у функции будте доступ

    chelovekMassiv[i].etaj = rand()%6;//задаем рандомную остановку при старрте

    chelovekMassiv[i].max_speed = 2 + rand()%9;
    chelovekMassiv[i].max_speed/=10;

    chelovekMassiv[i].direction = rand()%2;//задаем ранодомное направление
    chelovekMassiv[i].current_elev = rand()%2;
    if(chelovekMassiv[i].etaj == 0) chelovekMassiv[i].direction = 0;//если этаж - нижний, то направление - вперед
    if(chelovekMassiv[i].etaj == 5) chelovekMassiv[i].direction = 1;//если этаж - верхний, то направление - обратное
    
     if(chelovekMassiv[i].current_elev == 0) chelovekMassiv[i].MoveTo = etajMassiv[chelovekMassiv[i].etaj].level_first;//указываем точку движения персонажа соответствующие  
     if(chelovekMassiv[i].current_elev == 1) chelovekMassiv[i].MoveTo = etajMassiv[chelovekMassiv[i].etaj].level_second;//координаты остановочных мест 


    chelovekMassiv[i].MoveTo.x += -8 + rand()%16;//добавляем рандомный разброс, чтоб толпились возле лифта

    chelovekMassiv[i].position = etajMassiv[chelovekMassiv[i].etaj].position;//указываем начальные координаты самих персонажей
    chelovekMassiv[i].position.x += -128 + rand()%256;

    chelovekMassiv[i].walk = 1;//ставим флаг, что персоонаж движется к лифту

    while(1)//бесконечный цикл для непрерыной обработки персонажей
    {
    
        if(chelovekMassiv[i].walk)//елси текущий флаг - движение к лифту
        {
            coord dir_vector = {chelovekMassiv[i].MoveTo.x-chelovekMassiv[i].position.x, chelovekMassiv[i].MoveTo.y-chelovekMassiv[i].position.y};//рассчитываем вектор для движеня персонажа
            float l = sqrt(pow(dir_vector.x,2)+pow(dir_vector.y, 2));//рассчитываем длину этого вектора
            coord normal_v  = dir_vector;//нормализованный вектор, который должен иметь то же направление, но длинну 1
            normal_v.x /= l;//для этого делим его на длину первого вектора
            normal_v.y /= l;
            if(chelovekMassiv[i].speed < chelovekMassiv[i].max_speed && chelovekMassiv[i].speed < l/20)//если скорость персонажа меньше достаточно млаа
            {
                chelovekMassiv[i].speed += 0.01;//то постеменно ее увеличиваем
            }
            if(chelovekMassiv[i].speed > l/20)//если скорость слишком велика по отношению к оставшемуся расстоянию до цели, 
            {
                chelovekMassiv[i].speed = l/20;//то снижаем ее
            }

            chelovekMassiv[i].position.x += normal_v.x * chelovekMassiv[i].speed;//изменяем координаты персонажа на основании его скорости и нормализованного вектора длвижения
            chelovekMassiv[i].position.y += normal_v.y * chelovekMassiv[i].speed;


            if(chelovekMassiv[i].speed < 0.001)//если персонаж остановился
            {
                chelovekMassiv[i].walk = 0;//сбрасываем флаг, что он движется к остановке
                chelovekMassiv[i].wait = 1;//устанавливаем флаг ожидания транспорта
            }
        }

        if(chelovekMassiv[i].wait > 0)//если активен флаг ожидания
        {
            //смотрим, не переполнен ли лифт, остановился ли он, и находится ли лифт на той же остановке, что и персонаж
            if(chelovekMassiv[i].direction == liftMassiv[chelovekMassiv[i].current_elev].direction && chelovekMassiv[i].etaj == liftMassiv[chelovekMassiv[i].current_elev].etaj && liftMassiv[chelovekMassiv[i].current_elev].chelCount < 3)
            {
                chelovekMassiv[i].vLift = 1;//елси это так, то устанавливаем флаг, что персонаж движется к лифту
                chelovekMassiv[i].wait = 0;//сбрасываем флаг ожидания транспорта
                chelovekMassiv[i].MoveTo = liftMassiv[chelovekMassiv[i].current_elev].position;//утсанавливаем точку перемещения персонжа - координаты лифта
            }
        }

        if(chelovekMassiv[i].vLift == 1)//если активен флаг передвижения к лифту
        {
            if(liftMassiv[chelovekMassiv[i].current_elev].chelCount < 4 && liftMassiv[chelovekMassiv[i].current_elev].wait > 0)//если лифт все еще ждет и там есть свободные места
            {
                coord dir_vector = {chelovekMassiv[i].MoveTo.x-chelovekMassiv[i].position.x, chelovekMassiv[i].MoveTo.y-chelovekMassiv[i].position.y};//активируем алгоритм перемещения
                float l = sqrt(pow(dir_vector.x,2)+pow(dir_vector.y, 2));
                coord normal_v  = dir_vector;
                normal_v.x /= l;
                normal_v.y /= l;
                if(chelovekMassiv[i].speed < chelovekMassiv[i].max_speed && chelovekMassiv[i].speed < l/20)
                {
                    chelovekMassiv[i].speed += 0.01;
                }
                if(chelovekMassiv[i].speed > l/20)
                {
                    chelovekMassiv[i].speed = l/20;
                }

                chelovekMassiv[i].position.x += normal_v.x * chelovekMassiv[i].speed;
                chelovekMassiv[i].position.y += normal_v.y * chelovekMassiv[i].speed;


                if(chelovekMassiv[i].speed < 0.01)//и если человек почти дошел
                {
                    pthread_mutex_lock(&hmtx);//захватываем мьютексб на доступ к лифту
                    chelovekMassiv[i].drive = 1;//устанавливаем флаг езды
                    liftMassiv[chelovekMassiv[i].current_elev].chelCount++;//увеличиваем у лифта информацию о количестве пассажирова
                    chelovekMassiv[i].vLift = 0;//сбрасываем флаг движения к лифту
                    pthread_mutex_unlock(&hmtx);//отпускаем мьютекс
                }
            }else{//елси лифт уже поехал, или там уже закончились места
                chelovekMassiv[i].walk = 1;//идем обратно на туже остановку
                chelovekMassiv[i].vLift = 0;//сбарсываем флаг движения к лифту
                if(chelovekMassiv[i].current_elev == 0)//в зависимости от текущего направления, в котром решил ехать персонаж
                {
                    chelovekMassiv[i].MoveTo = etajMassiv[chelovekMassiv[i].etaj].level_first;//выбираем координаты нужного остановочного места как цлеевые/
                    chelovekMassiv[i].MoveTo.x += -8 + rand()%16;//добавляем немного рандома для создания толпы
                }
                if(chelovekMassiv[i].current_elev == 1)//в зависимости от текущего направления, в котром решил ехать персонаж
                {
                    chelovekMassiv[i].MoveTo = etajMassiv[chelovekMassiv[i].etaj].level_second;//выбираем координаты нужного остановочного места как цлеевые/
                    chelovekMassiv[i].MoveTo.x += -8 + rand()%16;//рандом для толпы
                }
            }
        }
        if(chelovekMassiv[i].drive == 1)//есди установлен флаг движени в лифту
        {
            chelovekMassiv[i].position = liftMassiv[chelovekMassiv[i].current_elev].position;//приравнивем координаты пассажира к координатам лифта
            if(liftMassiv[chelovekMassiv[i].current_elev].wait && chelovekMassiv[i].etaj != liftMassiv[chelovekMassiv[i].current_elev].etaj)//еслли лифт остановился, и этаж не равна предыдущей остановке, на котором былл человек
            {
                chelovekMassiv[i].etaj = liftMassiv[chelovekMassiv[i].current_elev].etaj;//обновляем информацию о текущей остановке персонажа
                int r = rand()%2;//делаем рандом который вернет 1 или 0
                if(r == 1)//елс 1 то 
                {
                    pthread_mutex_lock(&hmtx);//захватываем мьютекс
                    chelovekMassiv[i].drive = 0;//сбрасываем флаг езда на лифту
                    chelovekMassiv[i].fromLift = 1;//устанавливаем флаг движения от этажа
                    liftMassiv[chelovekMassiv[i].current_elev].chelCount--;//уменьшаем информациию о количестве пассажиров в  лифте
                    chelovekMassiv[i].MoveTo = etajMassiv[chelovekMassiv[i].etaj].position;//указываем целевую точку движения - координаты текущего этажа
                    chelovekMassiv[i].MoveTo.x += -64 + rand()%128;//и добавляем большой разброс в координтаах
                    if(chelovekMassiv[i].etaj == 0) chelovekMassiv[i].MoveTo.x += -512 + rand()%1024;
                    pthread_mutex_unlock(&hmtx);//отпускаем мьютекс
                }
            }
        }
        if(chelovekMassiv[i].fromLift == 1)//если установлен флаг движения в направлении  от становки
        {
            coord dir_vector = {chelovekMassiv[i].MoveTo.x-chelovekMassiv[i].position.x, chelovekMassiv[i].MoveTo.y-chelovekMassiv[i].position.y};//запускается алгорим перемещения
            float l = sqrt(pow(dir_vector.x,2)+pow(dir_vector.y, 2));
            coord normal_v  = dir_vector;
            normal_v.x /= l;
            normal_v.y /= l;
            if(chelovekMassiv[i].speed < chelovekMassiv[i].max_speed && chelovekMassiv[i].speed < l/20)
            {
                chelovekMassiv[i].speed += 0.01;
            }
            if(chelovekMassiv[i].speed > l/20)
            {
                chelovekMassiv[i].speed = l/20;
            }

            chelovekMassiv[i].position.x += normal_v.x * chelovekMassiv[i].speed;
            chelovekMassiv[i].position.y += normal_v.y * chelovekMassiv[i].speed;


            if(chelovekMassiv[i].speed < 0.01)//елси дошел до цлеи
            {
                chelovekMassiv[i].fromLift = 0;//сбрасыаем флаг движения  от этажа
                chelovekMassiv[i].sleep = 1;//устанавливаем флаг сна
                usleep(100000*(rand()%100 + 100));//засыпаем на неопределенное количество времени
                chelovekMassiv[i].sleep = 0;//после сна сбрасываем флаг сна
                chelovekMassiv[i].walk = 1;//устанавливаем флаг движения к остановке снова
                
                chelovekMassiv[i].direction = rand()%2;//делаем рандомное направление в котором хочет ехть человек
                chelovekMassiv[i].current_elev = rand()%2;
                if(chelovekMassiv[i].etaj == 0) chelovekMassiv[i].direction = 0;//елси текущий этаж - крайняя  то точно указываем направление в котором вообще можно ехать
                if(chelovekMassiv[i].etaj == 5) chelovekMassiv[i].direction = 1;
                if(chelovekMassiv[i].current_elev == 0)//в зависимости от выбранного направления 
                {
                    chelovekMassiv[i].MoveTo = etajMassiv[chelovekMassiv[i].etaj].level_first;//выбираем нужное остановочное место
                    chelovekMassiv[i].MoveTo.x += -8 + rand()%16;//и добавляем рандом для создания толп
                }
                if(chelovekMassiv[i].current_elev == 1)
                {
                    chelovekMassiv[i].MoveTo = etajMassiv[chelovekMassiv[i].etaj].level_second;
                    chelovekMassiv[i].MoveTo.x += -8 + rand()%16;
                }
            }
        }
        usleep(1000000/target_framerate);//отправляем поток в сон на 1 шестидесятую секунды.
    }
}


void showContent()//фнукция для отображения всего происходящего через окно
{
    XClearWindow(dspl, hwnd);//сначал очищщаем экран
    XSetForeground(dspl, gc, jolty.pixel);//устанавливаем цвет рисования - желтый
    
    XFillRectangle(dspl, hwnd, gc, liftMassiv[0].position.x-8, liftMassiv[0].position.y - 16, 16, 32);//заполняем прямоугльник лифту желтым
    XFillRectangle(dspl, hwnd, gc, liftMassiv[1].position.x-8, liftMassiv[1].position.y - 16, 16, 32);
   
    coord op;//вспомогательная координата предыдущей этажа для рисования дороги 
    for(int i = 0; i < 6; i++)//для всех четырех остановок
    {
        XSetForeground(dspl, gc, krasny.pixel);//устанавливаем красный цвет рисования
        coord p1 = etajMassiv[i].level_second;//берем координты остановчного места для обратного движения
        XDrawRectangle(dspl, hwnd, gc, p1.x-level_size/2, p1.y-level_size/2, level_size, level_size);//и рисуем эту осттановку

        
        
        XSetForeground(dspl, gc, SvZelen.pixel);//для остновочного место с обычным направлением движения берем зеленую кисть
        coord p2 = etajMassiv[i].level_first;//и выбираем координаты нужного остановочного метса
        XDrawRectangle(dspl, hwnd, gc, p2.x-level_size/2, p2.y-level_size/2, level_size, level_size);

        coord p = etajMassiv[i].position;//вспомогательная координата текущей этажа

        XSetForeground(dspl, gc, sery.pixel);//устанавливаем ссерый цвет рисования
        XDrawLine(dspl, hwnd, gc, p.x-256, p1.y+level_size/2, p.x+256, p1.y+level_size/2);//и рисуем эту осттановку
        char buf[1];
        sprintf(buf, "%d", i+1);
        XDrawString(dspl, hwnd, gc, p.x-256,  p1.y, buf,1);
    }
   /* char pc[4] = "";
    sprintf(pc, "%d", N);//преобразуем количество людей в строку*/

    for(int i = 0; i < N; i++)//проходим циклом по всему ссписку людей в масиве
    {
        if(chelovekMassiv[i].sleep == 1) XSetForeground(dspl, gc, sery.pixel);//елси человек спит, то рисуем его серым 
        else XSetForeground(dspl, gc, BlackPixel(dspl,screen));//иначи рисуем белым
        XFillArc(dspl, hwnd, gc, chelovekMassiv[i].position.x - 4, chelovekMassiv[i].position.y - 12+4, 8, 24, 0,360*64);//куржок символизирующий персонажа
        XFillArc(dspl, hwnd, gc, chelovekMassiv[i].position.x - 4, chelovekMassiv[i].position.y - 20+4, 8, 8, 0,360*64);//куржок символизирующий персонажа
    }

    XFlush(dspl);// отправление графической информации в само окно на вывод
}


void liftThread(void *arg)
{
    while(1)//запускаем безконечный цикл
    {
        for(int j = 0; j < 2; j++)
        {
            if(liftMassiv[j].drive == 1)//в нем проверяем флаг езды, и еси он 1, то
            {
                coord dir_vector = {liftMassiv[j].MoveTo.x-liftMassiv[j].position.x, liftMassiv[j].MoveTo.y-liftMassiv[j].position.y};//выполняем перемещение так же как и у пассажиров, только с более высокой скоростью
                float l = sqrt(pow(dir_vector.x,2)+pow(dir_vector.y, 2));
                coord normal_v  = dir_vector;
                normal_v.x /= l;
                normal_v.y /= l;
                if(liftMassiv[j].speed < 10 && liftMassiv[j].speed < l/20)//сдесь ограничение скорости выставленно в 2 раза выше чем у человеков
                {
                    liftMassiv[j].speed += 0.05;
                }
                if(liftMassiv[j].speed > l/20)
                {
                    liftMassiv[j].speed = l/20;
                }

                liftMassiv[j].position.x += normal_v.x * liftMassiv[j].speed;
                liftMassiv[j].position.y += normal_v.y * liftMassiv[j].speed;


                if(liftMassiv[j].speed < 0.001)//елси лифту почти полностью осттановилось, то 
                {
                    liftMassiv[j].drive = 0;//останавливаем его полностью, сбрасывае флаг езды
                    liftMassiv[j].wait = 360;//устанавливая вемя ожиданя
                    if(liftMassiv[j].direction == 0)//дальше проверяем, направление движени
                    {
                        liftMassiv[j].etaj++;//прибавляем 1 к информации о ткущей остановке лифту
                        if(liftMassiv[j].etaj == 5) //еси она оказалась на крайней остановке
                        {
                            liftMassiv[j].direction = 1;//изменяем направление движения на обратное
                        }
                    }else{//тут вспе тоже самое для только для обратного направления движения
                        liftMassiv[j].etaj--;
                        if(liftMassiv[j].etaj == 0) 
                        {  
                            liftMassiv[j].direction = 0;
                        }
                    }      
                }
            }
            if(liftMassiv[j].wait > 0)//елси еще не закончислоь время ожидания
            {
                liftMassiv[j].wait--;//то ждем,и немного это времяуменьшаем
                if(liftMassiv[j].wait == 0) //елси время истекло
                { 
                    liftMassiv[j].drive = 1;//устанавливаем флаг езды
                    int c;
                    if(liftMassiv[j].direction == 0)//в зависимости от направления движения
                    {
                        c = liftMassiv[j].etaj + 1;////выбираем слудующую останвоку
                    }
                    if(liftMassiv[j].direction == 1)
                    {
                        c = liftMassiv[j].etaj - 1;
                    }
                    liftMassiv[j].MoveTo.y = etajMassiv[c].position.y;//передаем ее координаты в целевую точку для движения
                   if(j == 0) liftMassiv[j].MoveTo.x = etajMassiv[c].position.x - 192;
                   else liftMassiv[j].MoveTo.x = etajMassiv[c].position.x + 192;
                }
            }
        }
        usleep(1000000/target_framerate);//отправляем основной поток вв сон на одну шестидесятую секунды
    }
}


void main()
{

    dspl = XOpenDisplay(NULL);//создание окна
    gc = XDefaultGC(dspl,0);//дескриптор для рисования в графическом окне
    if(dspl == 0) {printf("Error XOpenDisplay\n"); exit(1);}//выводим ошибку если не получиолсь соткрыть дисплей
    screen = XDefaultScreen(dspl);//сохранаяем инфо о экране
    hwnd = XCreateSimpleWindow(dspl, RootWindow(dspl, screen), 100,50,800,600,3, BlackPixel(dspl,screen), WhitePixel(dspl,screen));//созаем простое графическое окно с размерами 1200 на 600 

    Colormap screen_colormap;//созадем новую палитру цветов 
    

    screen_colormap = DefaultColormap(dspl, DefaultScreen(dspl));//копируем то что есть по умолчанию
    XAllocNamedColor(dspl, screen_colormap, "green", &SvZelen, &SvZelen);//у добавляем туда свои цвета 
    XAllocNamedColor(dspl, screen_colormap, "gold", &jolty, &jolty);
    XAllocNamedColor(dspl, screen_colormap, "web gray", &sery, &sery);
    XAllocNamedColor(dspl, screen_colormap, "krasny", &krasny, &krasny);
    XAllocNamedColor(dspl, screen_colormap, "white", &white, &white);
    XAllocNamedColor(dspl, screen_colormap, "dark green", &tZeleny, &tZeleny);
    XAllocNamedColor(dspl, screen_colormap, "sky siny", &siny, &siny);

    XSetForeground(dspl, gc, BlackPixel(dspl, screen));//утсанавливаем желлтый цвет рисования

    if(hwnd == 0) {printf("Error XCreateSimpleWindow\n"); exit(1);}//если произошли ошибки с окном то выводим предупреждение и выходим
    XSelectInput(dspl, hwnd, ExposureMask | KeyPressMask );//здес указываем какой ввод с мыши и клавиатуры мы будем считывать во время работы программы
    XMapWindow(dspl, hwnd);

    
    int n = 0;//флаг для ввода цифр
    char Nstr[4] = "";//строка для хранения вводимого числа
    while(n !=  -1)//пока не сброшен флаг 
    {
        XNextEvent(dspl, &event);//считыавем события от окна Х сревера
        if(event.type == KeyPress)//если событие - это нажатие клавиши на клавиатурпе
        {
            char kb[1];
            XLookupString(&event.xkey, kb,1, &key_sym, 0);//преобразуем его в сивол 
            char c = kb[0];
            int i = c - '0';//потом из символа в цифру
            if(i >= 0 && i <=9)//если она в пределах от 0 до 9 
            {
                XDrawString(dspl, hwnd, gc, 10 + n*10, 10, kb, 1);//выводим на дисплей его 
                Nstr[n] = kb[0];//записываем в сстроку
                n++;// увеличиваем флаг чисел
            }else{
                n = -1;//елси ввели не знам между 0 и 9 то сбрасываем флаг ввода чисел
                sscanf(Nstr, "%d", &N);//и сохраняем количество персонажей
            }
        }
    }

    Setlevels();//выполняем расположение остановк и остановочных мест в соответствии с координатами

    for(int i = 0; i < N; i++)//запускаем цикл на количество итераций равному числу, которое ввел польователь
    {
        people p;//создаем переменную для человека
        pthread_t thread;//перменную для потока
        pthread_create(&thread, NULL, (void*)chleThread, (void*)i);//запускаем поток
    }

    pthread_t thread;//перменную для потока
    pthread_create(&thread, NULL, (void*)liftThread, (void*)0);//запускаем поток


     
    //дальше идет алгоритм для маршрутного лифту
    elev1.drive = 1;//усатнавливаем, что при старте программы лифту едет
    elev1.direction = 1;//указываем направление (на первой же остановке оно поменяется)
    elev1.etaj = 1;//и сотановка
    elev1.MoveTo.y = etajMassiv[0].position.y;//укаываем начальную координату
    elev1.MoveTo.x = etajMassiv[0].position.x - 192;
    elev1.position.y = etajMassiv[0].position.y+1;//укаываем начальную координату
    elev1.position.x = etajMassiv[0].position.x - 192;

    elev2.drive = 1;//усатнавливаем, что при старте программы лифту едет
    elev2.direction = 1;//указываем направление (на первой же остановке оно поменяется)
    elev2.etaj = 2;//и сотановка
    elev2.MoveTo.y = etajMassiv[1].position.y;//укаываем начальную координату
    elev2.MoveTo.x = etajMassiv[1].position.x + 192;
    elev2.position.y = etajMassiv[1].position.y+1;//укаываем начальную координату
    elev2.position.x = etajMassiv[1].position.x + 192;

    liftMassiv[0] = elev1;
    liftMassiv[1] = elev2;
    
    while(1)
    {
        showContent();//выполняем отображене всех людей, лифту и остановоок в графическом окне
         usleep(1000000/target_framerate);//отправляем основной поток вв сон на одну шестидесятую секунды
    }

    getchar(); printf("\033[0m");
}