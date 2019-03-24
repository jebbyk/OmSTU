
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



void procThreadReader1(void *arg)
{
        pthread_mutex_lock(&hmtx);
      //  printf("\033[%d;0H\033[1;34m",1);
        pthread_mutex_unlock(&hmtx);
        usleep(2*speed);
}

int citySize = 256;
int vPadding = 32;
int hPadding = 64;
int vDistance = 256+128;
int hDistance = 512+256 + 128;
int rThickness = 32;

void DrawBack()
{
    XClearWindow(dspl,hwnd);
    
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

    XFlush(dspl);
    usleep(100);
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

    while(1){
        DrawBack();
    }
    

   // rc=pthread_create(&rdrThrd1, NULL, (void*)procThreadReader1, (void*)2);

    getchar(); printf("\033[0m");
}