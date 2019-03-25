
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

int passangersCount = 0;



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
int passangers;
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
        bussesList[arg] = b;
        usleep(1000000/50);
        if(b.y == b.ty && b.x == b.tx) 
        {
            b.waitingFlag = 1;
            bussesList[arg] = b;

            usleep(b.waitingTime);

            b.waitingFlag = 1;
            bussesList[arg] = b;
            if(b.dir2 == 0) 
            { 
                if(b.dir4 != 3) b.dir4++;
                else b.dir4 = 0;
                switch(b.dir4)
                {
                    case 0:
                    {
                        b.tx = hPadding + hDistance + citySize/2 - rThickness/4;
                        b.ty = vPadding + citySize/2 + rThickness/4;
                        break;
                    }
                    case 1:
                    {
                        b.tx = hPadding + hDistance + citySize/2 - rThickness/4;
                        b.ty = vPadding + citySize/2 + vDistance - rThickness/4;
                        break;
                    }
                    case 2:
                    {
                        b.tx = hPadding + citySize/2 + rThickness/4;
                        b.ty = vPadding + citySize/2 + vDistance - rThickness/4;
                        break;
                    }
                    case 3:
                    {
                        b.tx = hPadding + citySize/2 + rThickness/4;
                        b.ty = vPadding + citySize/2 + rThickness/4;
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
                        b.tx = hPadding + hDistance + citySize/2 + rThickness/4;
                        b.ty = vPadding + citySize/2 + vDistance + rThickness/4;
                        break;
                    }
                    case 1:
                    {
                        b.tx = hPadding + citySize/2 - rThickness/4;
                        b.ty = vPadding + citySize/2 + vDistance + rThickness/4;
                        break;
                    }
                    case 2:
                    {
                        b.tx = hPadding + citySize/2 - rThickness/4;
                        b.ty = vPadding + citySize/2 - rThickness/4;
                        break;
                    }
                    case 3:
                    {
                        b.tx = hPadding + citySize/2 + hDistance + rThickness/4;
                        b.ty = vPadding + citySize/2 - rThickness/4;
                        break;
                    }
                }
            }
        }
    }
}

struct passanger{
    int x, y, waitingFlag, maxStayTime, tx, ty, busNum, curCity;
}p_passanger;

struct passanger passangersList[8];

void ProcPassanger(void *_arg)
{
    int arg = (int)_arg;
    struct passanger p = passangersList[arg];
    int rCity = rand() % 5;//selekting start city
    p.curCity = rCity;
    p.x = cityList[rCity].x - citySize/2 + rand()%citySize;//setting rnd pos in city
    p.y = cityList[rCity].y - citySize/2 + rand()%citySize;


    int rS = rand() % p.maxStayTime;//setting wait time
    
    p.waitingFlag = 1;//wait
    passangersList[arg] = p;
    usleep(rS);
    p.waitingFlag = 0;
    passangersList[arg] = p;

    int rD = random()%2;//selecting rnd bus direction

    if(rD == 0) {p.tx = cityList[p.curCity].stopFx; p.ty = cityList[p.curCity].stopFy;}
    else {p.tx = cityList[p.curCity].stopBx; p.ty = cityList[p.curCity].stopBy;}

    if(p.x < p.tx) p.x++;//moving to the bus stop
    if(p.x > p.tx) p.x--;
    if(p.y < p.ty) p.y++;
    if(p.y > p.ty) p.y--;

}


void Draw(int sleepTime)
{
    XClearWindow(dspl, hwnd);
    char name1[] = "Dijon";
    XDrawRectangle(dspl, hwnd, gc, hPadding, vPadding, citySize, citySize);
    XDrawString(dspl, hwnd, gc, hPadding + 16, vPadding + 16, name1, 5);

    XDrawRectangle(dspl, hwnd, gc, hPadding + hDistance, vPadding, citySize, citySize);
    char name2[] = "Ruon";
    XDrawString(dspl, hwnd, gc, hPadding + hDistance + 16, vPadding + 16, name2, 4);

    XDrawRectangle(dspl, hwnd, gc, hPadding, vDistance + vPadding, citySize, citySize);
    char name3[] = "Paris";
    XDrawString(dspl, hwnd, gc, hPadding + 16, vPadding + vDistance + 16, name3, 5);

    
    XDrawRectangle(dspl, hwnd, gc, hPadding + hDistance, vPadding + vDistance,citySize, citySize);
    char name4[]= "Lion";
    XDrawString(dspl, hwnd, gc, hPadding + hDistance + 16, vPadding + vDistance + 16, name4, 4);

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

    
    while(passangersCount == 0)
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
            if(i > 0 && i < 10)
            {
                passangersCount = i;
            }
            
        }
    }


    struct bus busF;
    busF.x = hPadding + citySize/2;
    busF.y = vPadding + citySize/2 + rThickness/4;
    busF.tx = hPadding + citySize/2 + hDistance - rThickness/4;
    busF.ty = vPadding + citySize/2 + rThickness/4;
    busF.speed = 1;
    busF.dir2 = 0;
    busF.passangers = 0;
    busF.dir4 = 0;
    busF.waitingTime = 2000000;
    busF.waitingFlag = 1;
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
    busB.passangers = 0;
    busB.dir4 = 1;
    busB.waitingTime = 4000000;
    busB.waitingFlag = 0;
    bussesList[1] = busB;
    pthread_t busBthread;
    pthread_create(&busBthread, NULL, (void*)ProcBus, (void*)1);

    
    while(1)
    {
        Draw(1000000/50);//bigger number - lower FPS
    }

    getchar(); printf("\033[0m");
}