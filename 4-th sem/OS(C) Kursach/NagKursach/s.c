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

XColor l_green, d_green, white, gray, d_gray, yelow, red, blue;//используемые цвета
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
        levels_list[i].level_first.x = levels_list[i].position.x - 128;//координаты остановочных мест для каждого этажа (для движения в двух лифтах)
        levels_list[i].level_first.y = levels_list[i].position.y ;

        levels_list[i].level_second.x = levels_list[i].position.x  + 128;
        levels_list[i].level_second.y = levels_list[i].position.y;
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
elev elev2;
elev elev_list[2];

people people_list[1024];//массив содержащий множество структур со всеми людьми
short people_count;//переменная хранящая количесттво людей котрое ввел пользователь


void peopleThreadFunction(void *_i)//функция с алгоритмом поведения персонажей
{
    int i = _i;//сохраняемый переданный функции аргумент, это будет номер элемента массива с людьми, к оторому у функции будте доступ

    people_list[i].current_level = rand()%6;//задаем рандомную остановку при старрте

    people_list[i].direction = rand()%2;//задаем ранодомное направление
    people_list[i].current_elev = rand()%2;
    if(people_list[i].current_level == 0) people_list[i].direction = 0;//если этаж - нижний, то направление - вперед
    if(people_list[i].current_level == 5) people_list[i].direction = 1;//если этаж - верхний, то направление - обратное
    
     if(people_list[i].current_elev == 0) people_list[i].target_position = levels_list[people_list[i].current_level].level_first;//указываем точку движения персонажа соответствующие  
     if(people_list[i].current_elev == 1) people_list[i].target_position = levels_list[people_list[i].current_level].level_second;//координаты остановочных мест 


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
                    pthread_mutex_lock(&hmtx);//захватываем мьютексб на доступ к лифту
                    people_list[i].drive = 1;//устанавливаем флаг езды
                    elev_list[people_list[i].current_elev].people_amount++;//увеличиваем у лифта информацию о количестве пассажирова
                    people_list[i].walk_elev = 0;//сбрасываем флаг движения к лифту
                    pthread_mutex_unlock(&hmtx);//отпускаем мьютекс
                }
            }else{//елси лифт уже поехал, или там уже закончились места
                people_list[i].walk_level = 1;//идем обратно на туже остановку
                people_list[i].walk_elev = 0;//сбарсываем флаг движения к лифту
                if(people_list[i].current_elev == 0)//в зависимости от текущего направления, в котром решил ехать персонаж
                {
                    people_list[i].target_position = levels_list[people_list[i].current_level].level_first;//выбираем координаты нужного остановочного места как цлеевые/
                    people_list[i].target_position.x += -8 + rand()%16;//добавляем немного рандома для создания толпы
                    people_list[i].target_position.y += -8 + rand()%16;
                }
                if(people_list[i].current_elev == 1)//в зависимости от текущего направления, в котром решил ехать персонаж
                {
                    people_list[i].target_position = levels_list[people_list[i].current_level].level_second;//выбираем координаты нужного остановочного места как цлеевые/
                    people_list[i].target_position.x += -8 + rand()%16;//рандом для толпы
                    people_list[i].target_position.y += -8 + rand()%16;
                }
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
                    pthread_mutex_lock(&hmtx);//захватываем мьютекс
                    people_list[i].drive = 0;//сбрасываем флаг езда на лифту
                    people_list[i].walk_out = 1;//устанавливаем флаг движения от этажа
                    elev_list[people_list[i].current_elev].people_amount--;//уменьшаем информациию о количестве пассажиров в  лифте
                    people_list[i].target_position = levels_list[people_list[i].current_level].position;//указываем целевую точку движения - координаты текущего этажа
                    people_list[i].target_position.x += -64 + rand()%128;//и добавляем большой разброс в координтаах
                    if(people_list[i].current_level == 0) people_list[i].target_position.x += -512 + rand()%1024;
                    people_list[i].target_position.y += -16 + rand()%32;
                    pthread_mutex_unlock(&hmtx);//отпускаем мьютекс
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
                usleep(100000*(rand()%100 + 100));//засыпаем на неопределенное количество времени
                people_list[i].sleep = 0;//после сна сбрасываем флаг сна
                people_list[i].walk_level = 1;//устанавливаем флаг движения к остановке снова
                
                people_list[i].direction = rand()%2;//делаем рандомное направление в котором хочет ехть человек
                people_list[i].current_elev = rand()%2;
                if(people_list[i].current_level == 0) people_list[i].direction = 0;//елси текущий этаж - крайняя  то точно указываем направление в котором вообще можно ехать
                if(people_list[i].current_level == 5) people_list[i].direction = 1;
                if(people_list[i].current_elev == 0)//в зависимости от выбранного направления 
                {
                    people_list[i].target_position = levels_list[people_list[i].current_level].level_first;//выбираем нужное остановочное место
                    people_list[i].target_position.x += -8 + rand()%16;//и добавляем рандом для создания толпы
                    people_list[i].target_position.y += -8 + rand()%16;
                }
                if(people_list[i].current_elev == 1)
                {
                    people_list[i].target_position = levels_list[people_list[i].current_level].level_second;
                    people_list[i].target_position.x += -8 + rand()%16;
                    people_list[i].target_position.y += -8 + rand()%16;
                }
            }
        }
        usleep(1000000/target_framerate);//отправляем поток в сон на 1 шестидесятую секунды.
    }
}


void showContent()//фнукция для отображения всего происходящего через окно
{
    XClearWindow(dspl, hwnd);//сначал очищщаем экран
    XSetForeground(dspl, gc, yelow.pixel);//устанавливаем цвет рисования - желтый
    
    XDrawRectangle(dspl, hwnd, gc, elev_list[0].position.x-8, elev_list[0].position.y - 16, 16, 32);//заполняем прямоугльник лифту желтым
    XDrawRectangle(dspl, hwnd, gc, elev_list[1].position.x-8, elev_list[1].position.y - 16, 16, 32);
   
    coord op;//вспомогательная координата предыдущей этажа для рисования дороги 
    for(int i = 0; i < 6; i++)//для всех четырех остановок
    {
        XSetForeground(dspl, gc, red.pixel);//устанавливаем красный цвет рисования
        coord p1 = levels_list[i].level_second;//берем координты остановчного места для обратного движения
        XDrawRectangle(dspl, hwnd, gc, p1.x-level_size/2, p1.y-level_size/2, level_size, level_size);//и рисуем эту осттановку

        
        
        XSetForeground(dspl, gc, l_green.pixel);//для остновочного место с обычным направлением движения берем зеленую кисть
        coord p2 = levels_list[i].level_first;//и выбираем координаты нужного остановочного метса
        XDrawRectangle(dspl, hwnd, gc, p2.x-level_size/2, p2.y-level_size/2, level_size, level_size);

        coord p = levels_list[i].position;//вспомогательная координата текущей этажа

        XSetForeground(dspl, gc, d_gray.pixel);//устанавливаем красный цвет рисования
        XDrawRectangle(dspl, hwnd, gc, p.x-256, p1.y-level_size/2, 512, level_size);//и рисуем эту осттановку
        if(i > 0)
        {
            XDrawLine(dspl, hwnd, gc, op.x, op.y, p.x, p.y);//рисуем линию от предыдущей этажа к текущй
        }
        op = p;//координаты текущей этажа после рисованя становятся координатами предыдущей
    }
    char pc[4] = "";
    sprintf(pc, "%d", people_count);//преобразуем количество людей в строку

    for(int i = 0; i < people_count; i++)//проходим циклом по всему ссписку людей в масиве
    {
        if(people_list[i].sleep == 1) XSetForeground(dspl, gc, d_gray.pixel);//елси человек спит, то рисуем его серым 
        else XSetForeground(dspl, gc, white.pixel);//иначи рисуем белым
        XFillArc(dspl, hwnd, gc, people_list[i].position.x - 2, people_list[i].position.y - 2, 4, 4, 0,360*64);//куржок символизирующий персонажа
        char c[2] = "";
        sprintf(c, "%d", i);//преобразуем текущий номер элемента массива в букву 
        XSetForeground(dspl, gc, white.pixel);//устанавливаем цвет рисования - белый
        XDrawString(dspl, hwnd, gc, 820, 16 + i * 18, c, 2);//пишем номер человека в нужных координатах
        XSetForeground(dspl, gc, d_gray.pixel);//устанавливаем цвет рисованя - серый
        if(people_list[i].walk_level == 1) {//если человек сейчкс идет к остановке
            XSetForeground(dspl, gc, d_green.pixel);//устанавливаем темно зеленый цвет рисования
            XDrawString(dspl, hwnd, gc, 840, 16 + i * 18, "wk_st", 5);//выводим текст в таблицу
        }
        if(people_list[i].walk_elev == 1) {//делаем так же с остальными флагами в таблице, разукрашивая их в разные цвета для наглядности 
            XSetForeground(dspl, gc, yelow.pixel);
            XDrawString(dspl, hwnd, gc, 880, 16 + i * 18, "wk_el", 5);
        }
        if(people_list[i].walk_out == 1) {
            XSetForeground(dspl, gc, blue.pixel);
            XDrawString(dspl, hwnd, gc, 920, 16 + i * 18, "wk_ot", 5);
        }
        if(people_list[i].wait == 1) {
            XSetForeground(dspl, gc, d_gray.pixel);
            XDrawString(dspl, hwnd, gc, 960, 16 + i * 18, "wait", 4);
        }
        if(people_list[i].drive == 1) {
            XSetForeground(dspl, gc, yelow.pixel);
            XDrawString(dspl, hwnd, gc, 1000, 16 + i * 18, "drive", 5);
        }
        if(people_list[i].current_elev == 1) 
        {
            XSetForeground(dspl, gc, red.pixel);
            XDrawString(dspl, hwnd, gc, 1060, 16 + i * 18, "scnd", 4);
        }
        if(people_list[i].current_elev == 0) 
        {
            XSetForeground(dspl, gc, l_green.pixel);
            XDrawString(dspl, hwnd, gc, 1100, 16 + i * 18, "frst", 4);
        }
         XSetForeground(dspl, gc, d_green.pixel);
         char num[1] = "";
         sprintf(num, "%d", people_list[i].current_level);
         XDrawString(dspl, hwnd, gc, 1140 + people_list[i].current_level * 10, 16 + i * 18, num, 1);//отображение номера текущей этажа

    }

    XFlush(dspl);// отправление графической информации в само окно на вывод
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
                   if(j == 0) elev_list[j].target_position.x = levels_list[c].position.x - 192;
                   else elev_list[j].target_position.x = levels_list[c].position.x + 192;
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
    hwnd = XCreateSimpleWindow(dspl, RootWindow(dspl, screen), 100,50,1200,600,3, WhitePixel(dspl,screen), BlackPixel(dspl,screen));//созаем простое графическое окно с размерами 1200 на 600 

    Colormap screen_colormap;//созадем новую палитру цветов 
    

    screen_colormap = DefaultColormap(dspl, DefaultScreen(dspl));//копируем то что есть по умолчанию
    XAllocNamedColor(dspl, screen_colormap, "green", &l_green, &l_green);//у добавляем туда свои цвета 
    XAllocNamedColor(dspl, screen_colormap, "gold", &yelow, &yelow);
    XAllocNamedColor(dspl, screen_colormap, "web gray", &d_gray, &d_gray);
    XAllocNamedColor(dspl, screen_colormap, "red", &red, &red);
    XAllocNamedColor(dspl, screen_colormap, "white", &white, &white);
    XAllocNamedColor(dspl, screen_colormap, "dark green", &d_green, &d_green);
    XAllocNamedColor(dspl, screen_colormap, "sky blue", &blue, &blue);

    XSetForeground(dspl, gc, yelow.pixel);//утсанавливаем желлтый цвет рисования
    XSetBackground(dspl, gc, BlackPixel(dspl, screen));//черный цвет фона

    if(hwnd == 0) {printf("Error XCreateSimpleWindow\n"); exit(1);}//если произошли ошибки с окном то выводим предупреждение и выходим
    XSelectInput(dspl, hwnd, ExposureMask | KeyPressMask );//здес указываем какой ввод с мыши и клавиатуры мы будем считывать во время работы программы
    XMapWindow(dspl, hwnd);

    
    int n = 0;//флаг для ввода цифр
    char people_count_string[4] = "";//строка для хранения вводимого числа
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
                people_count_string[n] = kb[0];//записываем в сстроку
                n++;// увеличиваем флаг чисел
            }else{
                n = -1;//елси ввели не знам между 0 и 9 то сбрасываем флаг ввода чисел
                sscanf(people_count_string, "%d", &people_count);//и сохраняем количество персонажей
            }
        }
    }

    Setlevels();//выполняем расположение остановк и остановочных мест в соответствии с координатами

    for(int i = 0; i < people_count; i++)//запускаем цикл на количество итераций равному числу, которое ввел польователь
    {
        people p;//создаем переменную для человека
        pthread_t thread;//перменную для потока
        pthread_create(&thread, NULL, (void*)peopleThreadFunction, (void*)i);//запускаем поток
    }

    pthread_t thread;//перменную для потока
    pthread_create(&thread, NULL, (void*)ElevThreadFunction, (void*)0);//запускаем поток


     
    //дальше идет алгоритм для маршрутного лифту
    elev1.drive = 1;//усатнавливаем, что при старте программы лифту едет
    elev1.direction = 1;//указываем направление (на первой же остановке оно поменяется)
    elev1.current_level = 1;//и сотановка
    elev1.target_position.y = levels_list[0].position.y;//укаываем начальную координату
    elev1.target_position.x = levels_list[0].position.x - 192;
    elev1.position.y = levels_list[0].position.y+1;//укаываем начальную координату
    elev1.position.x = levels_list[0].position.x - 192;

    elev2.drive = 1;//усатнавливаем, что при старте программы лифту едет
    elev2.direction = 1;//указываем направление (на первой же остановке оно поменяется)
    elev2.current_level = 1;//и сотановка
    elev2.target_position.y = levels_list[0].position.y;//укаываем начальную координату
    elev2.target_position.x = levels_list[0].position.x + 192;
    elev2.position.y = levels_list[0].position.y+1;//укаываем начальную координату
    elev2.position.x = levels_list[0].position.x + 192;

    elev_list[0] = elev1;
    elev_list[1] = elev2;
    
    while(1)
    {
        showContent();//выполняем отображене всех людей, лифту и остановоок в графическом окне
         usleep(1000000/target_framerate);//отправляем основной поток вв сон на одну шестидесятую секунды
    }

    getchar(); printf("\033[0m");
}