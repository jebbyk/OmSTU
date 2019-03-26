
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <X11/Xlib.h>


Display *dspl;
int screen;
Window hwnd;
XEvent event;
KeySym ks;
GC gc;
int x, y,w,h;
int count = 0;
int speed = 100000;

int roadTickness = 16;


pthread_t rdrThrd1;
pthread_mutex_t hmtx;

int passengersCount = 0;



int citySize = 256;
int vPadding = 32;
int hPadding = 64;
int vDistance = 256+128;
int hDistance = 512+256 + 128;
int rThickness = 32;

struct city{
    int x, y;
    int stopFx, stopFy;
    int stopBx, stopBy;
};

struct city cityList[4];




struct bus{
int dir4;//world dirrection
int dir2;//dirrection on route
int x, y;
int tx, ty;
int passengers;
int speed;
int waitingTime;
int waitingFlag;
} b_bus;


int bussesCount = 2;//must bee the same
struct bus bussesList[2];//SAAAAAAMEEEEE!!!!

void ProcBus(void *_arg)
{
    int arg  = (int)_arg;
    struct bus b = bussesList[arg];
    while(1)
    {
        if(b.x < b.tx) b.x += b.speed; 
        if(b.y < b.ty) b.y += b.speed; 
        if(b.x > b.tx) b.x -= b.speed; 
        if(b.y > b.ty) b.y -= b.speed; 
        if(b.y == b.ty && b.x == b.tx) 
        {
            b.waitingFlag = 1;
            bussesList[arg] = b;

            usleep(b.waitingTime);

            b.waitingFlag = 0;
            bussesList[arg] = b;
            if(b.dir2 == 0) 
            { 
                if(b.dir4 != 3) b.dir4++;
                else b.dir4 = 0;
                switch(b.dir4)
                {
                    case 0:
                    {
                        b.tx = cityList[1].x - rThickness/4;
                        b.ty = cityList[1].y + rThickness/4;
                        break;
                    }
                    case 1:
                    {
                        b.tx = cityList[2].x - rThickness/4;
                        b.ty = cityList[2].y - rThickness/4;
                        break;
                    }
                    case 2:
                    {
                        b.tx = cityList[3].x + rThickness/4;
                        b.ty = cityList[3].y - rThickness/4;
                        break;
                    }
                    case 3:
                    {
                        b.tx = cityList[0].x + rThickness/4;
                        b.ty = cityList[0].y + rThickness/4;
                        break;
                    }
                }
            }else{
                if(b.dir4 != 0) b.dir4--;
                else b.dir4 = 3;
                switch(b.dir4)
                {
                    case 0:
                    {
                        b.tx = cityList[2].x + rThickness/4;
                        b.ty = cityList[2].y + rThickness/4;
                        break;
                    }
                    case 1:
                    {
                        b.tx = cityList[3].x - rThickness/4;
                        b.ty = cityList[3].y + rThickness/4;
                        break;
                    }
                    case 2:
                    {
                        b.tx = cityList[0].x - rThickness/4;
                        b.ty = cityList[0].y - rThickness/4;
                        break;
                    }
                    case 3:
                    {
                        b.tx = cityList[1].x + rThickness/4;
                        b.ty = cityList[1].y - rThickness/4;
                        break;
                    }
                }
            }
        }
        bussesList[arg] = b;
        usleep(1000000/50);
    }
}



struct passenger{
    int x, y, tx, ty, sleep, maxSleepTime, sleepTime, wait, drive, toStop, toCity, toBus, curCity, curDir, canLeave;
};

struct passenger passengersList[512];

void ProcPassenger(void *_arg)
{
    int i = (int)_arg;
    struct passenger p;
    p = passengersList[i];

    int rS = rand()%4;
    p.curCity = rS;
    p.x = cityList[rS].x - citySize/2  + rand()%citySize;
    p.y = cityList[rS].y - citySize/2  + rand()%citySize;


    int rD = rand()%2;
    p.curDir = rD;
    if(p.curDir == 0)
    {
        p.tx = cityList[p.curCity].stopFx - 8 + rand()%16;
        p.ty = cityList[p.curCity].stopFy - 8 + rand()%16;
    }else{
        p.tx = cityList[p.curCity].stopBx - 8 + rand()%16;
        p.ty = cityList[p.curCity].stopBy - 8 + rand()%16;
    }


    p.sleepTime = p.maxSleepTime/2 + rand()%p.maxSleepTime/2;
    p.sleepTime = p.sleepTime * 1000;
    while(1)
    {
        struct bus b;
        b = bussesList[p.curDir];


        if(p.toStop == 1)
        {
            if(p.x < p.tx) p.x++;
            if(p.x > p.tx) p.x--;
            if(p.y < p.ty) p.y++;
            if(p.y > p.ty) p.y--;

            if(p.x == p.tx && p.y == p.ty){
                p.toStop = 0;
                p.wait = 1;
            }
        }

        if(p.wait)
        {
            if(abs(p.x - b.x) < 64 && abs(p.y - b.y) < 64 && b.waitingFlag == 1)
            {
                p.tx = b.x; p.ty = b.y;
                p.wait = 0;
                p.toBus  = 1;
            }
        }

        if(p.toBus == 1)
        {
            if(b.waitingFlag == 1)
            {
                if(p.x < p.tx) p.x++;
                if(p.x > p.tx) p.x--;
                if(p.y < p.ty) p.y++;
                if(p.y > p.ty) p.y--;

                if(p.x == p.tx && p.y == p.ty){
                    p.drive = 1;
                    p.canLeave = 0;
                    p.toBus = 0;
                }
            }else{
                if(p.curDir == 0)
                {
                    p.tx = cityList[p.curCity].stopFx - 8 + rand()%16;
                    p.ty = cityList[p.curCity].stopFy - 8 + rand()%16;
                }else{
                    p.tx = cityList[p.curCity].stopBx - 8 + rand()%16;
                    p.ty = cityList[p.curCity].stopBy - 8 + rand()%16;
                }
                p.toStop = 1;
                p.toBus = 0;
            }
        }

        if(p.drive == 1)
        {
            p.x = b.x;
            p.y = b.y;
            if(b.waitingFlag == 0) p.canLeave = 1;
            if(b.waitingFlag == 1 && p.canLeave ==1)
            {
                p.drive = 0;
                p.toCity = 1;
                if(p.curDir == 0)
                {
                    if(p.curCity != 3) p.curCity++;
                    else p.curCity = 0;
                }else{
                    if(p.curCity != 0) p.curCity--;
                    else p.curCity = 3;
                }
                p.tx = cityList[p.curCity].x - citySize/2  + rand()%citySize;
                p.ty = cityList[p.curCity].y - citySize/2  + rand()%citySize;
            }
        }

        if(p.toCity)
        {
            if(p.x < p.tx) p.x++;
            if(p.x > p.tx) p.x--;
            if(p.y < p.ty) p.y++;
            if(p.y > p.ty) p.y--;

            if(p.x == p.tx && p.y == p.ty){
                p.sleep = 1;
                passengersList[i] = p;

                usleep(p.sleepTime);

                p.sleep = 0;

                p.toCity = 0;
                p.toStop = 1;
                p.wait = 0;

                rD = rand()%2;
                p.curDir = rD;
                if(p.curDir == 0)
                {
                    p.tx = cityList[p.curCity].stopFx - 8 + rand()%16;
                    p.ty = cityList[p.curCity].stopFy - 8 + rand()%16;
                }else{
                    p.tx = cityList[p.curCity].stopBx - 8 + rand()%16;
                    p.ty = cityList[p.curCity].stopBy - 8 + rand()%16;
                }
            }
        }



        passengersList[i] = p;
        usleep(1000000/50);//fps
    }

    
}



void Draw(int sleepTime)
{
    XClearWindow(dspl, hwnd);
    
    
    char pc[3];
    sprintf(pc, "%d", passengersCount);
    XDrawString(dspl,hwnd, gc, 20, 20, pc, 3);

    for(int i = 0; i < 4; i++)
    {
        struct city c = cityList[i];
        XDrawRectangle(dspl, hwnd, gc, c.x - citySize/2, c.y - citySize/2, citySize, citySize);
        
        char d[1];
        d[0] = 'f';
        XDrawRectangle(dspl, hwnd, gc, c.stopFx - 8, c.stopFy - 8, 16, 16);
        XDrawString(dspl,hwnd, gc, c.stopFx, c.stopFy-8, d, 1);
        
        d[0] = 'b';
        XDrawRectangle(dspl, hwnd, gc, c.stopBx - 8, c.stopBy - 8, 16, 16);
        XDrawString(dspl,hwnd, gc, c.stopBx, c.stopBy-8, d, 1);

        char cn[1];
        sprintf(cn, "%d", i);
        XDrawString(dspl,hwnd, gc, c.x, c.y - citySize/4, cn, 1);
    }

    XDrawRectangle(dspl, hwnd, gc, hPadding + citySize/2 - rThickness/2, vPadding + citySize/2 - rThickness/2, hDistance + rThickness, rThickness);
    XDrawRectangle(dspl, hwnd, gc, hPadding + citySize/2 - rThickness/2, vPadding + citySize/2 - rThickness/2, rThickness, vDistance + rThickness);
    XDrawRectangle(dspl, hwnd, gc, hPadding + citySize/2 - rThickness/2, vPadding + citySize/2 + vDistance - rThickness/2, hDistance + rThickness, rThickness);
    XDrawRectangle(dspl, hwnd, gc, hPadding + citySize/2 - rThickness/2 + hDistance, vPadding + citySize/2 - rThickness/2, rThickness, vDistance + rThickness);


    for(int i = 0; i < bussesCount; i++)
    {
        struct bus b = bussesList[i];
        if(b.dir4 == 0 || b.dir4 == 2)
        {
            XDrawRectangle(dspl, hwnd, gc, b.x - 8, b.y -  4, 16, 8);
        }else
        {
            XDrawRectangle(dspl, hwnd, gc, b.x - 4, b.y - 8, 8, 16);
        } 
        char d[1];
        if(b.waitingFlag == 0) d[0] = '.';
        else d[0] = 'W';
        XDrawString(dspl,hwnd, gc, b.x, b.y, d, 1);
    }

    for(int i = 0; i < passengersCount; i++)
    {
        struct passenger p = passengersList[i];
        XDrawRectangle(dspl, hwnd, gc, p.x-2, p.y-2, 4,4);
    }
    XFlush(dspl);
    usleep(sleepTime);
}


void main()
{
    struct city c1;
    c1.x = hPadding + citySize/2;
    c1.y = vPadding + citySize/2;
    c1.stopFx = c1.x + rThickness;
    c1.stopFy = c1.y + rThickness;
    c1.stopBx = c1.x;
    c1.stopBy = c1.y - rThickness;

    struct city c2;
    c2.x = hPadding + hDistance + citySize/2;
    c2.y = vPadding + citySize/2;
    c2.stopFx = c2.x - rThickness;
    c2.stopFy = c2.y + rThickness;
    c2.stopBx = c2.x + rThickness ;
    c2.stopBy = c2.y;

    struct city c3;
    c3.x = hPadding + hDistance + citySize/2;
    c3.y = vPadding + vDistance + citySize/2;
    c3.stopFx = c3.x - rThickness;
    c3.stopFy = c3.y - rThickness;
    c3.stopBx = c3.x;
    c3.stopBy = c3.y + rThickness;

    struct city c4;
    c4.x = hPadding + citySize/2;
    c4.y = vPadding + vDistance + citySize/2;
    c4.stopFx = c4.x + rThickness;
    c4.stopFy = c4.y - rThickness;
    c4.stopBx = c4.x - rThickness;
    c4.stopBy = c4.y;

    cityList[0] = c1;
    cityList[1] = c2;
    cityList[2] = c3;
    cityList[3] = c4;



    dspl = XOpenDisplay(NULL);
    gc = XDefaultGC(dspl,0);
    if(dspl == 0) {printf("Error XOpenDisplay\n"); exit(1);}
    screen = XDefaultScreen(dspl);
    hwnd = XCreateSimpleWindow(dspl, RootWindow(dspl, screen), 100,50,1280,720,3, BlackPixel(dspl,screen), WhitePixel(dspl,screen));
    if(hwnd == 0) {printf("Error XCreateSimpleWindow\n"); exit(1);}
    XSelectInput(dspl, hwnd, ExposureMask | KeyPressMask );//input mask
    XMapWindow(dspl, hwnd);

    
    while(passengersCount == 0)
    {
        char buf[]= "Enter number of passangers!";
        XDrawString(dspl, hwnd, gc, 10, 10, buf, 27);
        XNextEvent(dspl, &event);
        if(event.type == KeyPress)
        {
            XClearWindow(dspl,hwnd);
            char kb[1];
            XLookupString(&event.xkey, kb,1, &ks, 0);
            XDrawString(dspl, hwnd, gc, 10, 10, kb, 1);
            char c = kb[0];
            int i = c - '0';
            if(i >= 0 && i < 10)
            {
                int r = 1;
                for(int c = 0; c < i; c++ )
                {
                    r*=2;
                }
                passengersCount = r;
            }
            
        }
    }


    struct bus busF;
    busF.x = hPadding + citySize/2;
    busF.y = vPadding + citySize/2 + rThickness/4;
    busF.tx = hPadding + citySize/2 + hDistance - rThickness/4;
    busF.ty = vPadding + citySize/2 + rThickness/4;
    busF.speed = 4;
    busF.dir2 = 0;
    busF.passengers = 0;
    busF.dir4 = 0;
    busF.waitingTime = 2000000;
    busF.waitingFlag = 0;
    int rc;
    bussesList[0] = busF;
    pthread_t busFthread;
    pthread_create(&busFthread, NULL, (void*)ProcBus, (void*)0);


    struct bus busB;
    busB.x = hPadding + citySize/2 - rThickness/4;
    busB.y = vPadding + citySize/2;
    busB.tx = hPadding + citySize/2 - rThickness/4;
    busB.ty = vPadding + citySize/2 + vDistance + rThickness/4;
    busB.dir2 = 1;
    busB.speed = 2;
    busB.passengers = 0;
    busB.dir4 = 1;
    busB.waitingTime = 4000000;
    busB.waitingFlag = 0;
    bussesList[1] = busB;
    pthread_t busBthread;
    pthread_create(&busBthread, NULL, (void*)ProcBus, (void*)1);


    for(int i = 0; i < passengersCount; i++)
    {
        struct passenger p;
        p.x=0;
        p.y =0;
        p.tx = 0;
        p.ty = 0;
        p.curCity = 0;
        p.drive = 0;
        p.sleep = 0;
        p.maxSleepTime = 64000;
        p.sleepTime = 0;
        p.toCity = 0;
        p.toStop = 1;
        p.wait = 0;
      
        passengersList[i] = p;
        pthread_t p1Thread;
        pthread_create(&p1Thread, NULL, (void*)ProcPassenger, (void*)i);
    }

    while(1)
    {
        Draw(1000000/50);//bigger number - lower FPS
    }

    getchar(); printf("\033[0m");
}