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
pthread_mutex_t hmtx;//мьютекс для блокировки доступа к такси

XColor l_green, d_green, white, gray, d_gray, yelow, red, blue;//используемые цвета
unsigned int stop_size  = 16;//размер остановки

typedef struct{//структура для координат
    float x;
    float y;
}coord;


typedef struct {//структура для описсания остановки
    coord position;//координаты остановки
    coord stop_frwd;//координаты остановочного места в одном направлении
    coord stop_bkwd;//координаты остановочного места в обратном направлении
}stop;

stop stops_list[4];

void SetStops()//расположение всех остановок и остановочных мест для каждой остановки
{
    stops_list[0].position.x = 64;//координаты самих остановок
    stops_list[0].position.y = 200;

    stops_list[1].position.x = 300;
    stops_list[1].position.y = 128;

    stops_list[2].position.x = 600;
    stops_list[2].position.y = 300;

    stops_list[3].position.x = 756;
    stops_list[3].position.y = 150;

    for(int i = 0; i < 4; i++)
    {
        stops_list[i].stop_frwd.x = stops_list[i].position.x;//координаты остановочных мест для каждой остановки (для движения в двух направлениях)
        stops_list[i].stop_frwd.y = stops_list[i].position.y - 16;

        stops_list[i].stop_bkwd.x = stops_list[i].position.x;
        stops_list[i].stop_bkwd.y = stops_list[i].position.y + 16;
    }   
}


typedef struct{//структура описывающая персонажей
    coord position;//текущая позиция персонажа
    coord target_position;//кординаты точки, куда нужно идти человеку
    short drive;//флаг говорящий о том что человвек едет
    short sleep;//спит
    short walk_stop;//идет к остановке
    short walk_taxi;//идет к такси
    short walk_out;//идет от остановки
    short wait;//ждет
    short direction;//направление в котором хочет ехать человек
    short curent_stop;//текущая остановка, на которой находится человек
    float speed;//текущая скорость перемещения человека
}people;

typedef struct{//структура описывающая такси
    coord position;//текущая позиция
    coord target_position;//точка в которую едет такси
    float speed;//его скорость
    short drive;//флаг, который указывает, едет ли сейчас такси
    short wait;//ждет на остановке
    short people_amount;//количество лудй внутри
    short curent_stop;//текущая остановка, на которой находится такси
    short direction;//направление движения такси
}taxi;

taxi taxi1;//само такси

people people_list[1024];//массив содержащий множество структур со всеми людьми
short people_count;//переменная хранящая количесттво людей котрое ввел пользователь


void peopleThreadFunction(void *_i)//функция с алгоритмом поведения персонажей
{
    int i = _i;//сохраняемый переданный функции аргумент, это будет номер элемента массива с людьми, к оторому у функции будте доступ

    people_list[i].curent_stop = rand()%4;//задаем рандомную остановку при старрте

    people_list[i].direction = rand()%2;//задаем ранодомное направление
    if(people_list[i].curent_stop == 0) people_list[i].direction = 0;//если остановка - крайняя левая, то направление - вперед
    if(people_list[i].curent_stop == 3) people_list[i].direction = 1;//если остановка - крайняя правая, то направление - обратное
    
     if(people_list[i].direction == 0) people_list[i].target_position = stops_list[people_list[i].curent_stop].stop_frwd;//указываем точку движения персонажа соответствующие  
     if(people_list[i].direction == 1) people_list[i].target_position = stops_list[people_list[i].curent_stop].stop_bkwd;//координаты остановочных мест 


    people_list[i].target_position.x += -8 + rand()%16;//добавляем рандомный разброс, чтоб толпились на остановке
    people_list[i].target_position.y += -8 + rand()%16;

    people_list[i].position = stops_list[people_list[i].curent_stop].position;//указываем начальные координаты самих персонажей
    people_list[i].position.x += -64 + rand()%128;
    people_list[i].position.y += -64 + rand()%128;

    people_list[i].walk_stop = 1;//ставим флаг, что персоонаж движется к остановке

    while(1)//бесконечный цикл для непрерыной обработки персонажей
    {
    
        if(people_list[i].walk_stop)//елси текущий флаг - движение к остановке
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
                people_list[i].walk_stop = 0;//сбрасываем флаг, что он движется к остановке
                people_list[i].wait = 1;//устанавливаем флаг ожидания транспорта
            }
        }

        if(people_list[i].wait > 0)//если активен флаг ожидания
        {
            //смотрим, не переполнено ли такси, остановилось ли оно, и находится ли такси на той же остановке, что и персонаж
            if(people_list[i].direction == taxi1.direction && people_list[i].curent_stop == taxi1.curent_stop && taxi1.people_amount < 4)
            {
                people_list[i].walk_taxi = 1;//елси это так, то устанавливаем флаг, что персонаж движется к такси
                people_list[i].wait = 0;//сбрасываем флаг ожидания транспорта
                people_list[i].target_position = taxi1.position;//утсанавливаем точку перемещения персонжа - координаты такси
            }
        }

        if(people_list[i].walk_taxi == 1)//если активен флаг передвижения к такси
        {
            if(taxi1.people_amount < 4 && taxi1.wait > 0)//если такси все еще ждет и там есть свободные места
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
                    pthread_mutex_lock(&hmtx);//захватываем мьютексб на доступ к такси
                    people_list[i].drive = 1;//устанавливаем флаг езды
                    taxi1.people_amount++;//увеличиваем у такси информацию о количестве пассажирова
                    people_list[i].walk_taxi = 0;//сбрасываем флаг движения к такси
                    pthread_mutex_unlock(&hmtx);//отпускаем мьютекс
                }
            }else{//елси такси уже поехало, или там уже закончились места
                people_list[i].walk_stop = 1;//идем обратно на туже остановку
                people_list[i].walk_taxi = 0;//сбарсываем флаг движения к такси
                if(people_list[i].direction == 0)//в зависимости от текущего направления, в котром решил ехать персонаж
                {
                    people_list[i].target_position = stops_list[people_list[i].curent_stop].stop_frwd;//выбираем координаты нужного остановочного места как цлеевые/
                    people_list[i].target_position.x += -8 + rand()%16;//добавляем немного рандома для создания толпы
                    people_list[i].target_position.y += -8 + rand()%16;
                }
                if(people_list[i].direction == 1)//в зависимости от текущего направления, в котром решил ехать персонаж
                {
                    people_list[i].target_position = stops_list[people_list[i].curent_stop].stop_bkwd;//выбираем координаты нужного остановочного места как цлеевые/
                    people_list[i].target_position.x += -8 + rand()%16;//рандом для толпы
                    people_list[i].target_position.y += -8 + rand()%16;
                }
            }
        }
        if(people_list[i].drive == 1)//есди установлен флаг движени в такси
        {
            people_list[i].position = taxi1.position;//приравнивем координаты пассажира к координатам такси
            if(taxi1.wait && people_list[i].curent_stop != taxi1.curent_stop)//еслли такси остановилось, и остановка не равна предыдущей остановке, на которой былл человек
            {
                people_list[i].curent_stop = taxi1.curent_stop;//обновляем информацию о текущей остановке персонажа
                int r = rand()%2;//делаем рандом который вернет 1 или 0
                if(r == 1)//елс 1 то 
                {
                    pthread_mutex_lock(&hmtx);//захватываем мьютекс
                    people_list[i].drive = 0;//сбрасываем флаг езда на такси
                    people_list[i].walk_out = 1;//устанавливаем флаг движения от остановки
                    taxi1.people_amount--;//уменьшаем информациию о количестве пассажиров в  такси
                    people_list[i].target_position = stops_list[people_list[i].curent_stop].position;//указываем целевую точку движения - координаты текущей остановки
                    people_list[i].target_position.x += -64 + rand()%128;//и добавляем большой разброс в координтаах
                    people_list[i].target_position.y += -64 + rand()%128;
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
                people_list[i].walk_out = 0;//сбрасыаем флаг движения  от остановки
                people_list[i].sleep = 1;//устанавливаем флаг сна
                usleep(100000*(rand()%100 + 100));//засыпаем на неопределенное количество времени
                people_list[i].sleep = 0;//после сна сбрасываем флаг сна
                people_list[i].walk_stop = 1;//устанавливаем флаг движения к остановке снова
                
                people_list[i].direction = rand()%2;//делаем рандомное направление в котором хочет ехть человек
                if(people_list[i].curent_stop == 0) people_list[i].direction = 0;//елси текущая остановка - крайняя  то точно указываем направление в котором вообще можно ехать
                if(people_list[i].curent_stop == 3) people_list[i].direction = 1;
                if(people_list[i].direction == 0)//в зависимости от выбранного направления 
                {
                    people_list[i].target_position = stops_list[people_list[i].curent_stop].stop_frwd;//выбираем нужное остановочное место
                    people_list[i].target_position.x += -8 + rand()%16;//и добавляем рандом для создания толпы
                    people_list[i].target_position.y += -8 + rand()%16;
                }
                if(people_list[i].direction == 1)
                {
                    people_list[i].target_position = stops_list[people_list[i].curent_stop].stop_bkwd;
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
    
    XFillRectangle(dspl, hwnd, gc, taxi1.position.x-8, taxi1.position.y - 4, 16, 8);//заполняем прямоугльник такси желтым
    
   
    coord op;//вспомогательная координата предыдущей остановки для рисования дороги 
    for(int i = 0; i < 4; i++)//для всех четырех остановок
    {
        XSetForeground(dspl, gc, red.pixel);//устанавливаем красный цвет рисования
        coord p1 = stops_list[i].stop_bkwd;//берем координты остановчного места для обратного движения
        XDrawRectangle(dspl, hwnd, gc, p1.x-stop_size/2, p1.y-stop_size/2, stop_size, stop_size);//и рисуем эту осттановку
        
        XSetForeground(dspl, gc, l_green.pixel);//для остновочного место с обычным направлением движения берем зеленую кисть
        coord p2 = stops_list[i].stop_frwd;//и выбираем координаты нужного остановочного метса
        XDrawRectangle(dspl, hwnd, gc, p2.x-stop_size/2, p2.y-stop_size/2, stop_size, stop_size);

        coord p = stops_list[i].position;//вспомогательная координата текущей остановки
        XSetForeground(dspl, gc, d_gray.pixel);//выбираем серую кистьдля рисования дороги
        if(i > 0)
        {
            XDrawLine(dspl, hwnd, gc, op.x, op.y, p.x, p.y);//рисуем линию от предыдущей остановки к текущй
        }
        op = p;//координаты текущей остановки после рисованя становятся координатами предыдущей
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
        if(people_list[i].walk_stop == 1) {//если человек сейчкс идет к остановке
            XSetForeground(dspl, gc, d_green.pixel);//устанавливаем темно зеленый цвет рисования
            XDrawString(dspl, hwnd, gc, 840, 16 + i * 18, "wk_st", 5);//выводим текст в таблицу
        }
        if(people_list[i].walk_taxi == 1) {//делаем так же с остальными флагами в таблице, разукрашивая их в разные цвета для наглядности 
            XSetForeground(dspl, gc, yelow.pixel);
            XDrawString(dspl, hwnd, gc, 880, 16 + i * 18, "wk_tx", 5);
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
        if(people_list[i].direction == 1) 
        {
            XSetForeground(dspl, gc, red.pixel);
            XDrawString(dspl, hwnd, gc, 1060, 16 + i * 18, "bkwd", 4);
        }
        if(people_list[i].direction == 0) 
        {
            XSetForeground(dspl, gc, l_green.pixel);
            XDrawString(dspl, hwnd, gc, 1100, 16 + i * 18, "frwd", 4);
        }
         XSetForeground(dspl, gc, d_green.pixel);
         char num[1] = "";
         sprintf(num, "%d", people_list[i].curent_stop);
         XDrawString(dspl, hwnd, gc, 1140 + people_list[i].curent_stop * 12, 16 + i * 18, num, 1);//отображение номера текущей остановки

    }

    XFlush(dspl);// отправление графической информации в само окно на вывод
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

    SetStops();//выполняем расположение остановк и остановочных мест в соответствии с координатами

    for(int i = 0; i < people_count; i++)//запускаем цикл на количество итераций равному числу, которое ввел польователь
    {
        people p;//создаем переменную для человека
        pthread_t thread;//перменную для потока
        pthread_create(&thread, NULL, (void*)peopleThreadFunction, (void*)i);//запускаем поток
    }


     
    //дальше идет алгоритм для маршрутного такси
    taxi1.drive = 1;//усатнавливаем, что при старте программы такси едет
    taxi1.direction = 1;//указываем направление (на первой же остановке оно поменяется)
    taxi1.curent_stop = 1;//и сотановка
    taxi1.target_position = stops_list[0].position;//укаываем начальную координату
    while(1)//запускаем безконечный цикл
    {
        if(taxi1.drive == 1)//в нем проверяем флаг езды, и еси он 1, то
        {
            coord dir_vector = {taxi1.target_position.x-taxi1.position.x, taxi1.target_position.y-taxi1.position.y};//выполняем перемещение так же как и у пассажиров, только с более высокой скоростью
            float l = sqrt(pow(dir_vector.x,2)+pow(dir_vector.y, 2));
            coord normal_v  = dir_vector;
            normal_v.x /= l;
            normal_v.y /= l;
            if(taxi1.speed < 10 && taxi1.speed < l/20)//сдесь ограничение скорости выставленно в 2 раза выше чем у человеков
            {
                taxi1.speed += 0.05;
            }
            if(taxi1.speed > l/20)
            {
                taxi1.speed = l/20;
            }

            taxi1.position.x += normal_v.x * taxi1.speed;
            taxi1.position.y += normal_v.y * taxi1.speed;


            if(taxi1.speed < 0.001)//елси такси почти полностью осттановилось, то 
            {
                taxi1.drive = 0;//останавливаем его полностью, сбрасывае флаг езды
                taxi1.wait = 240;//устанавливая вемя ожиданя
                if(taxi1.direction == 0)//дальше проверяем, направление движени
                {
                    taxi1.curent_stop++;//прибавляем 1 к информации о ткущей остановке такси
                    if(taxi1.curent_stop == 3) //еси она оказалась на крайней остановке
                    {
                        taxi1.direction = 1;//изменяем направление движения на обратное
                    }
                }else{//тут вспе тоже самое для только для обратного направления движения
                    taxi1.curent_stop--;
                    if(taxi1.curent_stop == 0) 
                    {  
                        taxi1.direction = 0;
                    }
                }      
            }
        }
        if(taxi1.wait > 0)//елси еще не закончислоь время ожидания
        {
            taxi1.wait--;//то ждем,и немного это времяуменьшаем
            if(taxi1.wait == 0) //елси время истекло
            { 
                taxi1.drive = 1;//устанавливаем флаг езды
                int c;
                if(taxi1.direction == 0)//в зависимости от направления движения
                {
                    c = taxi1.curent_stop + 1;////выбираем слудующую останвоку
                }
                if(taxi1.direction == 1)
                {
                    c = taxi1.curent_stop - 1;
                }
                taxi1.target_position = stops_list[c].position;//передаем ее координаты в целевую точку для движения
            }
        }
        showContent();//выполняем отображене всех людей, такси и остановоок в графическом окне
        usleep(1000000/target_framerate);//отправляем основной поток вв сон на одну шестидесятую секунды
    }

    getchar(); printf("\033[0m");
}