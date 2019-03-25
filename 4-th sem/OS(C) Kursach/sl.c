
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

void DrawBack()
{
        
        
}

struct bus{
int dir4;//world dirrection
int dir2;//dirrection on route
int x, y;
int passangers;
int speed;
int waitingTime;
} b_bus;


int bussesCount = 2;//must bee the same
struct bus bussesList[2];//SAAAAAAMEEEEE!!!!

void ProcBus(void *_arg)
{
    int arg  = (int)_arg;
    struct bus b = bussesList[arg];
    while(1)
    {
        
        if(b.dir2 == 0) 
        { 
            if(b.dir4 == 0)
            {
                if(b.x < hPadding + citySize/2 + hDistance - rThickness/4)
                {
                        b.x += b.speed;
                        usleep(10000);
                }else{
                    usleep(b.waitingTime);
                    b.dir4++;
                }
            }
            if(b.dir4 == 1)
            {
                if(b.y < vPadding + citySize/2 + vDistance - rThickness/4)
                {
                        b.y += b.speed;
                        usleep(10000);
                }else{
                    usleep(b.waitingTime);
                    b.dir4++;
                }
            }
            if(b.dir4 == 2)
            {
                if(b.x > hPadding + citySize/2 + roadTickness/4)
                {
                        b.x -= b.speed;
                        usleep(10000);
                }else{
                    usleep(b.waitingTime);
                    b.dir4++;
                }
            }
            if(b.dir4 == 3)
            {
                if(b.y > vPadding + citySize/2 + rThickness/4)
                {
                        b.y -= b.speed;
                        usleep(10000);
                }else{
                    usleep(b.waitingTime);
                    b.dir4 = 0;
                }
            }
        }

        if(b.dir2 == 1) 
        { 
            if(b.dir4 == 0)
            {
                if(b.x < hPadding + citySize/2 + hDistance + rThickness/4)
                {
                        b.x += b.speed;
                        usleep(10000);
                }else{
                    usleep(b.waitingTime);
                    b.dir4 = 3;
                }
            }
            if(b.dir4 == 1)
            {
                if(b.y < vPadding + citySize/2 + vDistance + rThickness/4)
                {
                        b.y += b.speed;
                        usleep(10000);
                }else{
                    usleep(b.waitingTime);
                    b.dir4--;
                }
            }
            if(b.dir4 == 2)
            {
                if(b.x > hPadding + citySize/2 - rThickness/4)
                {
                        b.x -= b.speed;
                        usleep(10000);
                }else{
                    usleep(b.waitingTime);
                    b.dir4--;
                }
            }
            if(b.dir4 == 3)
            {
                if(b.y > vPadding + citySize/2 - rThickness/4)
                {
                        b.y -= b.speed;
                        usleep(10000);
                }else{
                    usleep(b.waitingTime);
                    b.dir4--;
                }
            }
        }
        bussesList[arg] = b;
    }
}

struct passanger{
    int x, y, waitingTime, curTime, waitingFlag;
}p_passanger;

struct passanger passangersList[8];

void ProcPassanger(int *arg)
{

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
    busF.speed = 1;
    busF.dir2 = 0;
    busF.passangers = 0;
    busF.dir4 = 0;
    busF.waitingTime = 2000000;
    int rc;
    bussesList[0] = busF;
    pthread_t busFthread;
    pthread_create(&busFthread, NULL, (void*)ProcBus, (void*)0);


    struct bus busB;
    busB.x = hPadding + citySize/2 - rThickness/4;
    busB.y = vPadding + citySize/2;
    busB.dir2 = 1;
    busB.speed = 2;
    busB.passangers = 0;
    busB.dir4 = 1;
    busB.waitingTime = 4000000;
    bussesList[1] = busB;
    pthread_t busBthread;
    pthread_create(&busBthread, NULL, (void*)ProcBus, (void*)1);

    
    while(1)
    {
        Draw(1000000/50);//bigger number - lower FPS
    }

    getchar(); printf("\033[0m");
}