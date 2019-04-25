#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <X11/Xlib.h>

#include <math.h>


Display *dspl;
int screen;
Window hwnd;
XEvent event;
KeySym ks;
GC gc;



pthread_mutex_t hmtx;

int roadT = 32;
typedef struct{
    int x, y;
    short dirs[4];
    short empty;
} cross;



cross crosses[2];

typedef struct{//структура с информацией о машинах
short dir;
int x, y;
int tx, ty;
short driveTo;
short driveOn;
short driveOut;
short wait;
short curCross;
} car;

car cars[8192];
int carsCount;
char carsCountC[2];

void ProcCar(void *_arg)
{
    int i  = (int)_arg;
    while(1)
    {
        if(cars[i].driveTo == 1)//если едет к перекрестку
        {
            if(crosses[cars[i].curCross].dirs[cars[i].dir] == 1)//првоерка того, что направление в котором движетсчя авто свободно
            {
                if(cars[i].dir == 0)
                {
                    if(cars[i].x < crosses[cars[i].curCross].x - roadT) 
                    {
                        short l = 1;
                        for(int j = 0; j < carsCount; j++)
                        {
                            if(j != i && cars[j].x > cars[i].x && cars[j].x - 16 < cars[i].x && cars[i].dir == cars[j].dir) //для того чтобы машины не наезжали друг на друга
                            { l = 0; break;}
                        }
                        if(l == 1)
                        {
                            cars[i].x++;
                        }
                    }
                    else{
                        pthread_mutex_lock(&hmtx);//синхронизация
                        crosses[cars[i].curCross].empty = 0;
                        crosses[cars[i].curCross].dirs[1] = 1;
                        crosses[cars[i].curCross].dirs[2] = 0;
                        crosses[cars[i].curCross].dirs[3] = 0;
                        cars[i].driveOn = 1;
                        cars[i].driveTo = 0;
                    }
                }
                if(cars[i].dir == 1)
                {
                    if(cars[i].x > crosses[cars[i].curCross].x + roadT)
                    {
                         short l = 1;
                        for(int j = 0; j < carsCount; j++)
                        {
                            if(j != i && cars[j].x < cars[i].x && cars[j].x + 16 > cars[i].x && cars[i].dir == cars[j].dir) 
                            { l = 0; break;}
                        }
                        if(l == 1)
                        {
                            cars[i].x--;
                        }
                    }
                    else{
                        pthread_mutex_lock(&hmtx);
                        crosses[cars[i].curCross].empty = 0;//когда заехал на перекресток обновляет ыинформацию о себе и о перекрестке
                        crosses[cars[i].curCross].dirs[0] = 1;
                        crosses[cars[i].curCross].dirs[2] = 0;
                        crosses[cars[i].curCross].dirs[3] = 0;
                        cars[i].driveOn = 1;
                        cars[i].driveTo = 0;
                    }
                }

                if(cars[i].dir == 2)//тоже самое для другого направления
                {
                    if(cars[i].y < crosses[cars[i].curCross].y - roadT) 
                    {
                         short l = 1;
                        for(int j = 0; j < carsCount; j++)
                        {
                            if(j != i && cars[j].y > cars[i].y && cars[j].y - 16 < cars[i].y && cars[i].dir == cars[j].dir) 
                            { l = 0; break;}
                        }
                        if(l == 1)
                        {
                            cars[i].y++;
                        }
                    }
                    else{
                        pthread_mutex_lock(&hmtx);
                        crosses[cars[i].curCross].empty = 0;
                        crosses[cars[i].curCross].dirs[0] = 0;
                        crosses[cars[i].curCross].dirs[1] = 0;
                        crosses[cars[i].curCross].dirs[3] = 1;
                        cars[i].driveOn = 1;
                        cars[i].driveTo = 0;
                    }
                }

                if(cars[i].dir == 3)
                {
                    if(cars[i].y > crosses[cars[i].curCross].y + roadT)
                    { 
                     short l = 1;
                        for(int j = 0; j < carsCount; j++)
                        {
                            if(j != i && cars[j].y < cars[i].y && cars[j].y + 16 > cars[i].y && cars[i].dir == cars[j].dir) 
                            { l = 0; break;}
                        }
                        if(l == 1)
                        {
                            cars[i].y--;
                        }
                    }
                    else{
                        pthread_mutex_lock(&hmtx);
                        crosses[cars[i].curCross].empty = 0;
                        crosses[cars[i].curCross].dirs[0] = 0;
                        crosses[cars[i].curCross].dirs[2] = 1;
                        crosses[cars[i].curCross].dirs[1] = 0;
                        cars[i].driveOn = 1;
                        cars[i].driveTo = 0;
                    }
                }
            }
        }

        if(cars[i].driveOn == 1)//если едет по перекрестку
        {
            if(cars[i].dir == 0)
            {
                if(cars[i].curCross == 0)
                {
                    if(cars[i].x < crosses[cars[i].curCross].x + roadT) cars[i].x++;
                    else{
                        crosses[cars[i].curCross].empty = 1;
                        for(int j = 0; j < 4; j++)
                        {
                            crosses[0].dirs[j] = 1;
                        }
                        pthread_mutex_unlock(&hmtx);//когда проехал его, то разлочит доступ кнему 
                        cars[i].driveOn = 0;
                        cars[i].driveTo = 1;
                        cars[i].curCross = 1;
                    }
                }else{
                    if(cars[i].x < crosses[cars[i].curCross].x + roadT) cars[i].x++;
                    else{
                        crosses[cars[i].curCross].empty = 1;
                        for(int j = 0; j < 4; j++)
                        {
                            crosses[1].dirs[j] = 1;
                        }
                        pthread_mutex_unlock(&hmtx);
                        cars[i].driveOn = 0;
                        cars[i].driveOut = 1;
                    }
                }
            }
            if(cars[i].dir == 1)//соже самое для остаьныъ направлений движения
            {
                if(cars[i].curCross == 1)
                {
                    if(cars[i].x  > crosses[cars[i].curCross].x - roadT) cars[i].x--;
                    else{
                        crosses[cars[i].curCross].empty = 1;
                        for(int j = 0; j < 4; j++)
                        {
                            crosses[1].dirs[j] = 1;
                        }
                        pthread_mutex_unlock(&hmtx);
                        cars[i].driveOn = 0;
                        cars[i].driveTo = 1;
                        cars[i].curCross = 0;
                    }
                }else{
                    if(cars[i].x > crosses[cars[i].curCross].x - roadT) cars[i].x--;
                    else{
                        crosses[cars[i].curCross].empty = 1;
                        for(int j = 0; j < 4; j++)
                        {
                            crosses[0].dirs[j] = 1;
                        }
                        pthread_mutex_unlock(&hmtx);
                        cars[i].driveOn = 0;
                        cars[i].driveOut = 1;
                    }
                }
            }

            if(cars[i].dir == 2)
            {
                if(cars[i].y < crosses[cars[i].curCross].y + roadT) cars[i].y++;
                else{
                    crosses[cars[i].curCross].empty = 1;
                    for(int j = 0; j < 4; j++)
                        {
                            crosses[cars[i].curCross].dirs[j] = 1;
                        }
                    pthread_mutex_unlock(&hmtx);
                    cars[i].driveOn = 0;
                    cars[i].driveOut = 1;
                }
            }

            if(cars[i].dir == 3)
            {
                if(cars[i].y > crosses[cars[i].curCross].y - roadT) cars[i].y--;
                else{
                    crosses[cars[i].curCross].empty = 1;
                    for(int j = 0; j < 4; j++)
                        {
                            crosses[cars[i].curCross].dirs[j] = 1;
                        }
                    pthread_mutex_unlock(&hmtx);
                    cars[i].driveOn = 0;
                    cars[i].driveOut = 1;
                }
            }
        }

         if(cars[i].driveOut == 1)//если едет от перекрестка
        {
            if(cars[i].dir == 0)
            {
                if(cars[i].x < 800) cars[i].x++;
                else{
                    cars[i].x = -32 - rand()%800;
                    cars[i].driveOut = 0;
                    cars[i].driveTo = 1;
                    cars[i].curCross = 0;
                }
            }
            if(cars[i].dir == 1)
            {
                if(cars[i].x > 0) cars[i].x--;
                else{
                    cars[i].x = 800 + rand()%800;
                    cars[i].driveOut = 0;
                    cars[i].driveTo = 1;
                    cars[i].curCross = 1;
                }
            }

            if(cars[i].dir == 2)
            {
                if(cars[i].y < 800) cars[i].y++;
                else{
                    cars[i].y = -32 - rand()%800;
                    cars[i].driveOut = 0;
                    cars[i].driveTo = 1;
                }
            }

            if(cars[i].dir == 3)
            {
                if(cars[i].y > 0) cars[i].y--;
                else{
                    cars[i].y = 800 + rand()%800;
                    cars[i].driveOut = 0;
                    cars[i].driveTo = 1;
                }
            }
        }

        usleep(100000/20);
    }
}



void Draw(int sleepTime)
{
    XClearWindow(dspl, hwnd);
    
    cross c = crosses[0];
    XDrawRectangle(dspl, hwnd, gc, c.x - roadT/2, 0, roadT, 800);//рисование дорог
    c = crosses[1];
    XDrawRectangle(dspl, hwnd, gc, c.x - roadT/2, 0, roadT, 800);
    XDrawRectangle(dspl, hwnd, gc, 0, c.y - roadT/2, 800, roadT);



    for(int i = 0; i < carsCount; i++)//рисование машин
    {
        car b = cars[i];
        if(b.dir == 0 || b.dir == 1)//depending on it's direction 
        {
            XDrawRectangle(dspl, hwnd, gc, b.x - 8, b.y -  4, 16, 8);//draw vertycal or horizontal
        }else
        {
            XDrawRectangle(dspl, hwnd, gc, b.x - 4, b.y - 8, 8, 16);
        } 
    }

    XFlush(dspl);
    usleep(1000000/20);//20 раз в секунду
}


void main()
{

    dspl = XOpenDisplay(NULL);//setting up new window
    gc = XDefaultGC(dspl,0);
    if(dspl == 0) {printf("Error XOpenDisplay\n"); exit(1);}
    screen = XDefaultScreen(dspl);
    hwnd = XCreateSimpleWindow(dspl, RootWindow(dspl, screen), 100,50,800,600,3, BlackPixel(dspl,screen), WhitePixel(dspl,screen));
    if(hwnd == 0) {printf("Error XCreateSimpleWindow\n"); exit(1);}
    XSelectInput(dspl, hwnd, ExposureMask | KeyPressMask );//input mask
    XMapWindow(dspl, hwnd);

    
    while(carsCount == 0)//inputs char from keyboard while it more then -1 and less then 10
    {
        char buf[]= "Enter number of cars!";
        XDrawString(dspl, hwnd, gc, 10, 10, buf, 23);
        XNextEvent(dspl, &event);
        if(event.type == KeyPress)
        {
            XClearWindow(dspl,hwnd);
            char kb[1];
            XLookupString(&event.xkey, kb,1, &ks, 0);//converts keyKode to char symbol
            XDrawString(dspl, hwnd, gc, 10, 10, kb, 1);
            char c = kb[0];
            int i = c - '0';
            if(i >= 0 && i < 10)
            {
                int r = 1;
                for(int c = 0; c < i; c++ )
                {
                    r*=2;//makes 2^inputedNumber to create more cars
                }
                carsCount = r;
            }
            
        }
    }

    cross cr1;
    cross cr2;
    crosses[0] = cr1;
    crosses[1] = cr2;

    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            crosses[i].dirs[j] = 1;
        }
    }

    crosses[0].x = 256;
    crosses[0].y = 256;
    crosses[1].x = 512;
    crosses[1].y = 256;



    for(int i = 0; i < carsCount; i++)//создаем нужное количество машин
    {
        car c;
        c.dir = rand()%4;
        if(c.dir == 0) 
        {
            c.curCross = 0;
            c.x = 0 - rand()%800;
            c.y = crosses[0].y + roadT/4;
        }

        if(c.dir == 1) 
        {
            c.curCross = 1;
            c.x = 800 + rand()%800;
            c.y = crosses[0].y  - roadT/4;
        }
        if(c.dir == 2) 
        {
            c.curCross = rand()%2;
            c.x = crosses[c.curCross].x - roadT/4;
            c.y = 0 - rand()%800;
        }
        if(c.dir == 3) 
        {
            c.curCross = rand()%2;
            c.x = crosses[c.curCross].x + roadT/4;
            c.y = 800 + rand()%800;
        }
        c.driveTo = 1;
        
      
        cars[i] = c;
        pthread_t p1Thread;
        pthread_create(&p1Thread, NULL, (void*)ProcCar, (void*)i);//starts thread with passenger
    }

    while(1)
    {
        Draw(1000000/60);//bigger number - lower FPS
    }

    getchar(); printf("\033[0m");
}