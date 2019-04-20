#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <X11/Xlib.h>

#include <math.h>

unsigned short target_framerate = 60;
Display *dspl;
int screen;
Window hwnd;
XEvent event;
KeySym ks;
GC gc;

unsigned short way_width;
unsigned short stop_size;

typedef struct{
    float x;
    float y;
}coord;

typedef struct{
    coord position;
    coord stop_frwd;
    coord stop_bkwd;
}stop;

stop stops_list[4];

void SetStops()
{
    stops_list[0].position.x = 128;
    stops_list[0].position.y = 256;

    stops_list[1].position.x = 256;
    stops_list[1].position.y = 128;

    stops_list[2].position.x = 384;
    stops_list[2].position.y = 400;

    stops_list[3].position.x = 512;
    stops_list[3].position.y = 200;

    for(int i = 0; i < 4; i++)
    {
        stops_list[i].stop_frwd.x = stops_list[i].position.x;
        stops_list[i].stop_frwd.y = stops_list[i].position.y - 32;

        stops_list[i].stop_bkwd.x = stops_list[i].position.x;
        stops_list[i].stop_bkwd.y = stops_list[i].position.y + 32;
    }   
}


typedef struct{
    coord position;
    coord target_position;
    short drive;
    short sleep;
    short walkStop;
    short walkTaxi;
    short walkOut;
}people;

typedef struct{
    coord position;
    coord target_postion;
    float speed;
    short drive;
    short wait;
    short people_amount;
}taxi;


void peopleThreadFunction(void *arg)
{
    unsigned short i = arg;
}


/*pthread_mutex_lock(&hmtx);
pthread_mutex_unlock(&hmtx);*/



void showContent()
{
    XClearWindow(dspl, hwnd);
    
    
   /* char pc[3];
    sprintf(pc, "%d", 123);//draws passenger coutn

    XDrawString(dspl,hwnd, gc, 20, 20, pc, 3);*/
    
    coord op;
    for(int i = 0; i < 4; i++)
    {
        coord p = stops_list[i].position;
        XDrawRectangle(dspl, hwnd, gc,   p.x-stop_size/2, p.y-stop_size/2,     p.x + (stop_size/2), p.y+(stop_size/2));
        
        if(i > 0)
        {
            XDrawLine(dspl, hwnd, gc, op.x, op.y, p.x, p.y);
        }
        op = p;
    }

    XFlush(dspl);
    usleep(1000000/target_framerate);
}


void main()
{
    /*char ch[32];
    strcpy(h, "Dijon");*/

    dspl = XOpenDisplay(NULL);//setting up new window
    gc = XDefaultGC(dspl,0);
    if(dspl == 0) {printf("Error XOpenDisplay\n"); exit(1);}
    screen = XDefaultScreen(dspl);
    hwnd = XCreateSimpleWindow(dspl, RootWindow(dspl, screen), 100,50,1280,720,3, BlackPixel(dspl,screen), WhitePixel(dspl,screen));
    if(hwnd == 0) {printf("Error XCreateSimpleWindow\n"); exit(1);}
    XSelectInput(dspl, hwnd, ExposureMask | KeyPressMask );//input mask
    XMapWindow(dspl, hwnd);

    
   /* while(passengersCount == 0)//inputs char from keyboard while it more then -1 and less then 10
    {
        char buf[]= "yyy";
        XNextEvent(dspl, &event);
        if(event.type == KeyPress)
        {
            XClearWindow(dspl,hwnd);
            char kb[1];
            XLookupString(&event.xkey, kb,1, &ks, 0);//converts keyKode to char symbol
            XDrawString(dspl, hwnd, gc, 10, 10, kb, 1);
            char c = kb[0];
            int i = c - '0';
        }
    }*/


   /* pthread_t busFthread;
    pthread_create(&busFthread, NULL, (void*)ProcBus, (void*)0);//starts thread with selected bus*/

    SetStops();

    while(1)
    {
       showContent();
    }

    getchar(); printf("\033[0m");
}