
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

int roadTickness = 16;

pthread_mutex_t hmtx;

int passengersCount = 0;



int citySize = 256;
int vPadding = 32;// distances from top of the window to the city edge
int hPadding = 64;//
int vDistance = 256+128;//distances between cities
int hDistance = 512+256 + 128;
int rThickness = 32;//thickness of roads

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
int tx, ty;//target coordinates
int passengers;//count
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
        if(b.x < b.tx) b.x += b.speed; //movement
        if(b.y < b.ty) b.y += b.speed; 
        if(b.x > b.tx) b.x -= b.speed; 
        if(b.y > b.ty) b.y -= b.speed; 
        if(b.y == b.ty && b.x == b.tx) 
        {
            b.waitingFlag = 1;
            pthread_mutex_lock(&hmtx);
            bussesList[arg] = b;
            pthread_mutex_unlock(&hmtx);

            usleep(b.waitingTime);//waits near the busStop

            pthread_mutex_lock(&hmtx);
            b = bussesList[arg];
            pthread_mutex_unlock(&hmtx);
            b.waitingFlag = 0;
            if(b.dir2 == 0) //forward or backwrd direction
            { 
                if(b.dir4 != 3) b.dir4++;
                else b.dir4 = 0;
                switch(b.dir4)//world dirrection
                {
                    case 0:
                    {
                        b.tx = cityList[1].x - rThickness/4;//setting target points
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
        pthread_mutex_lock(&hmtx);
        bussesList[arg] = b;//update bus info
        pthread_mutex_unlock(&hmtx);
        usleep(1000000/50);// do it 50 times in second
    }
}



struct passenger{
    //coordinates of passengers and some flags about passenger satate (what they doing)
    int x, y, tx, ty, sleep, maxSleepTime, sleepTime, wait, drive, toStop, toCity, toBus, curCity, curDir, canLeave;
};

struct passenger passengersList[512];

void ProcPassenger(void *_arg)
{
    int i = (int)_arg;
    struct passenger p;
    p = passengersList[i];

    int rS = rand()%4;//selecting random city
    p.curCity = rS;
    p.x = cityList[rS].x - citySize/2  + rand()%citySize;//setting random start position 
    p.y = cityList[rS].y - citySize/2  + rand()%citySize;


    int rD = rand()%2;//selecting random route direction
    p.curDir = rD;
    if(p.curDir == 0)
    {
        p.tx = cityList[p.curCity].stopFx - 8 + rand()%16;//setting random point on needed stop
        p.ty = cityList[p.curCity].stopFy - 8 + rand()%16;//depending on selected dirrection
    }else{
        p.tx = cityList[p.curCity].stopBx - 8 + rand()%16;
        p.ty = cityList[p.curCity].stopBy - 8 + rand()%16;
    }


    p.sleepTime = p.maxSleepTime/2 + rand()%p.maxSleepTime/2;//setting random sleepInCity time
    p.sleepTime = p.sleepTime * 1000;// rand generates not bigger then ~32000, so its just a mult to get bigger number
    while(1)
    {
        struct bus b;
        b = bussesList[p.curDir];


        if(p.toStop == 1)
        {
            if(p.x < p.tx) p.x++;//movement
            if(p.x > p.tx) p.x--;
            if(p.y < p.ty) p.y++;
            if(p.y > p.ty) p.y--;

            if(p.x == p.tx && p.y == p.ty){// if comes to stop then wait
                p.toStop = 0;
                p.wait = 1;
            }
        }

        if(p.wait)
        {
            if(abs(p.x - b.x) < 64 && abs(p.y - b.y) < 64 && b.waitingFlag == 1 && bussesList[p.curDir].passengers < 5)//if bus is close and it's waits
            {
                p.tx = b.x; p.ty = b.y;//start moving to bus
                p.wait = 0;
                p.toBus  = 1;
            }
        }

        if(p.toBus == 1)
        {
            if(b.waitingFlag == 1)//if buss waits
            {
                if(bussesList[p.curDir].passengers < 5)
                {
                    if(p.x < p.tx) p.x++;//move to it
                    if(p.x > p.tx) p.x--;
                    if(p.y < p.ty) p.y++;
                    if(p.y > p.ty) p.y--;
                }else{
                    if(p.curDir == 0)//returns to needed stop
                    {
                        p.tx = cityList[p.curCity].stopFx - 8 + rand()%16;//random position on busstop
                        p.ty = cityList[p.curCity].stopFy - 8 + rand()%16;
                    }else{
                        p.tx = cityList[p.curCity].stopBx - 8 + rand()%16;
                        p.ty = cityList[p.curCity].stopBy - 8 + rand()%16;
                    }
                    p.toStop = 1;
                    p.toBus = 0;
                }

                if(p.x == p.tx && p.y == p.ty){//if comes to bus then drive and can't leave in the same city
                    if(bussesList[p.curDir].passengers < 5)
                    {
                        p.drive = 1;
                        pthread_mutex_lock(&hmtx);
                        bussesList[p.curDir].passengers++;
                        pthread_mutex_unlock(&hmtx);
                        p.canLeave = 0;
                        p.toBus = 0;
                    }else{
                        if(p.curDir == 0)//returns to needed stop
                        {
                            p.tx = cityList[p.curCity].stopFx - 8 + rand()%16;//random position on busstop
                            p.ty = cityList[p.curCity].stopFy - 8 + rand()%16;
                        }else{
                            p.tx = cityList[p.curCity].stopBx - 8 + rand()%16;
                            p.ty = cityList[p.curCity].stopBy - 8 + rand()%16;
                        }
                        p.toStop = 1;
                        p.toBus = 0;
                    }
                }
            }else{//if bus not waiting
                if(p.curDir == 0)//returns to needed stop
                {
                    p.tx = cityList[p.curCity].stopFx - 8 + rand()%16;//random position on busstop
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
            p.x = b.x;//pos = bus pos
            p.y = b.y;
            if(b.waitingFlag == 0) p.canLeave = 1;//if bus not waits then can leave 
            if(b.waitingFlag == 1 && p.canLeave ==1)//(will leave only when bus stops
            {
                p.drive = 0;
                p.toCity = 1;
                if(bussesList[p.curDir].passengers > 0)
                {
                    pthread_mutex_lock(&hmtx);
                    bussesList[p.curDir].passengers--;
                    pthread_mutex_unlock(&hmtx);
                }
                if(p.curDir == 0)//update info about curent city depending on curentDirrection
                {
                    if(p.curCity != 3) p.curCity++;
                    else p.curCity = 0;
                }else{
                    if(p.curCity != 0) p.curCity--;
                    else p.curCity = 3;
                }
                p.tx = cityList[p.curCity].x - citySize/2  + rand()%citySize;//selecting random target point in curent city
                p.ty = cityList[p.curCity].y - citySize/2  + rand()%citySize;
            }
        }

        if(p.toCity)
        {
            if(p.x < p.tx) p.x++;//movement
            if(p.x > p.tx) p.x--;
            if(p.y < p.ty) p.y++;
            if(p.y > p.ty) p.y--;

            if(p.x == p.tx && p.y == p.ty){//if comes to target point then sleep some seconds
                p.sleep = 1;
                passengersList[i] = p;

                usleep(p.sleepTime);

                p.sleep = 0;

                p.toCity = 0;
                p.toStop = 1;
                p.wait = 0;

                rD = rand()%2;//select random direction (backwards or forwards)
                p.curDir = rD;
                if(p.curDir == 0)
                {
                    p.tx = cityList[p.curCity].stopFx - 8 + rand()%16;//and selcting target point on stop
                    p.ty = cityList[p.curCity].stopFy - 8 + rand()%16;
                }else{
                    p.tx = cityList[p.curCity].stopBx - 8 + rand()%16;
                    p.ty = cityList[p.curCity].stopBy - 8 + rand()%16;
                }
            }
        }

        

        passengersList[i] = p;
        usleep(1000000/20);

        /*int calcHardness = 1024;//                                  ///////////////////////////////////MEGACALCULATION\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
        int res = 0;
        for(int i = 1; i < calcHardness; i++)
        {
            for(int j = 1; j < calcHardness; j++)
            {
                res = i + j - i%j + j%i + i/j - j/i * 7;
            }
        }*///                                              \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\whill make your processor very very hot!!///////////////////////////////////
    }

    
}



void Draw(int sleepTime)
{
    XClearWindow(dspl, hwnd);
    
    
    char pc[3];
    sprintf(pc, "%d", passengersCount);//draws passenger coutn
    XDrawString(dspl,hwnd, gc, 20, 20, pc, 3);

    for(int i = 0; i < 4; i++)//draws cities
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
//draw roads
    XDrawRectangle(dspl, hwnd, gc, hPadding + citySize/2 - rThickness/2, vPadding + citySize/2 - rThickness/2, hDistance + rThickness, rThickness);
    XDrawRectangle(dspl, hwnd, gc, hPadding + citySize/2 - rThickness/2, vPadding + citySize/2 - rThickness/2, rThickness, vDistance + rThickness);
    XDrawRectangle(dspl, hwnd, gc, hPadding + citySize/2 - rThickness/2, vPadding + citySize/2 + vDistance - rThickness/2, hDistance + rThickness, rThickness);
    XDrawRectangle(dspl, hwnd, gc, hPadding + citySize/2 - rThickness/2 + hDistance, vPadding + citySize/2 - rThickness/2, rThickness, vDistance + rThickness);


    for(int i = 0; i < bussesCount; i++)//draw each bus
    {
        struct bus b = bussesList[i];
        if(b.dir4 == 0 || b.dir4 == 2)//depending on it's direction 
        {
            XDrawRectangle(dspl, hwnd, gc, b.x - 8, b.y -  4, 16, 8);//draw vertycal or horizontal
        }else
        {
            XDrawRectangle(dspl, hwnd, gc, b.x - 4, b.y - 8, 8, 16);
        } 
        char cn[1];
        sprintf(cn, "%d", b.passengers);
        XDrawString(dspl,hwnd, gc, b.x-2, b.y+4, cn, 1);
    }

    for(int i = 0; i < passengersCount; i++)//draw each passanger
    {
        struct passenger p = passengersList[i];
        if(p.sleep == 0 && p.drive  == 0)  XDrawRectangle(dspl, hwnd, gc, p.x-2, p.y-2, 4,4);
    }
    XFlush(dspl);
    usleep(1000000/50);
}


void main()
{
    struct city c1;//setting cityes positions 
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



    dspl = XOpenDisplay(NULL);//setting up new window
    gc = XDefaultGC(dspl,0);
    if(dspl == 0) {printf("Error XOpenDisplay\n"); exit(1);}
    screen = XDefaultScreen(dspl);
    hwnd = XCreateSimpleWindow(dspl, RootWindow(dspl, screen), 100,50,1280,720,3, BlackPixel(dspl,screen), WhitePixel(dspl,screen));
    if(hwnd == 0) {printf("Error XCreateSimpleWindow\n"); exit(1);}
    XSelectInput(dspl, hwnd, ExposureMask | KeyPressMask );//input mask
    XMapWindow(dspl, hwnd);

    
    while(passengersCount == 0)//inputs char from keyboard while it more then -1 and less then 10
    {
        char buf[]= "Enter number of passangers!";
        XDrawString(dspl, hwnd, gc, 10, 10, buf, 27);
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
                    r*=2;//makes 2^inputedNumber to create more passangers
                }
                passengersCount = r;
            }
            
        }
    }


    struct bus busF;//creating two busses
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
    pthread_create(&busFthread, NULL, (void*)ProcBus, (void*)0);//starts thread with selected bus


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


    for(int i = 0; i < passengersCount; i++)//creats needed count of passengers
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
        pthread_create(&p1Thread, NULL, (void*)ProcPassenger, (void*)i);//starts thread with passenger
    }

    while(1)
    {
        Draw(1000000/50);//bigger number - lower FPS
    }

    getchar(); printf("\033[0m");
}