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
pthread_mutex_t hmtx;

unsigned int way_width;
unsigned int stop_size  = 16;

typedef struct{
    float x;
    float y;
}coord;


typedef struct {
    coord position;
    coord stop_frwd;
    coord stop_bkwd;
}stop;

stop stops_list[4];

void SetStops()
{
    stops_list[0].position.x = 64;
    stops_list[0].position.y = 200;

    stops_list[1].position.x = 300;
    stops_list[1].position.y = 128;

    stops_list[2].position.x = 600;
    stops_list[2].position.y = 300;

    stops_list[3].position.x = 756;
    stops_list[3].position.y = 150;

    for(int i = 0; i < 4; i++)
    {
        stops_list[i].stop_frwd.x = stops_list[i].position.x;
        stops_list[i].stop_frwd.y = stops_list[i].position.y - 16;

        stops_list[i].stop_bkwd.x = stops_list[i].position.x;
        stops_list[i].stop_bkwd.y = stops_list[i].position.y + 16;
    }   
}


typedef struct{
    coord position;
    coord target_position;
    short drive;
    short sleep;
    short walk_stop;
    short walk_taxi;
    short walk_out;
    short wait;
    short direction;
    short curent_stop;
    float speed;
}people;

typedef struct{
    coord position;
    coord target_position;
    float speed;
    short drive;
    short wait;
    short people_amount;
    short curent_stop;
    short direction;
}taxi;

taxi taxi1;

people people_list[1024];
short people_count;


void peopleThreadFunction(void *_i)
{
    int i = _i;
    srand(i);
    people_list[i].direction = rand()%2;

    if(people_list[i].direction == 0)
    {
        people_list[i].target_position = stops_list[people_list[i].curent_stop].stop_frwd;
        people_list[i].target_position.x += -12 + rand()%24;
        people_list[i].target_position.y += -12 + rand()%24;
    }
    if(people_list[i].direction == 1)
    {
        people_list[i].target_position = stops_list[people_list[i].curent_stop].stop_bkwd;
        people_list[i].target_position.x += -12 + rand()%24;
        people_list[i].target_position.y += -12 + rand()%24;
    }
        people_list[i].position = stops_list[people_list[i].curent_stop].position;
        people_list[i].position.x += -64 + rand()%128;
        people_list[i].position.y += -64 + rand()%128;

    people_list[i].walk_stop = 1;

    while(1)
    {
    
        if(people_list[i].walk_stop)
        {
            coord dir_vector = {people_list[i].target_position.x-people_list[i].position.x, people_list[i].target_position.y-people_list[i].position.y};
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


            if(people_list[i].speed < 0.001)
            {
                people_list[i].walk_stop = 0;
                people_list[i].wait = 1;
            }
        }

        if(people_list[i].wait > 0)
        {
            if(people_list[i].direction == taxi1.direction && people_list[i].curent_stop == taxi1.curent_stop && taxi1.people_amount < 4)
            {
                people_list[i].walk_taxi = 1;
                people_list[i].wait = 0;
                people_list[i].target_position = taxi1.position;
            }
        }

        if(people_list[i].walk_taxi == 1)
        {
            if(taxi1.people_amount < 4 && taxi1.wait > 0)
            {
                coord dir_vector = {people_list[i].target_position.x-people_list[i].position.x, people_list[i].target_position.y-people_list[i].position.y};
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


                if(people_list[i].speed < 0.01)
                {
                    pthread_mutex_lock(&hmtx);
                    people_list[i].drive = 1;
                    taxi1.people_amount++;
                    people_list[i].walk_taxi = 0;
                    pthread_mutex_unlock(&hmtx);
                }
            }else{
                people_list[i].walk_stop = 1;
                people_list[i].walk_taxi = 0;
                if(people_list[i].direction == 0)
                {
                    people_list[i].target_position = stops_list[people_list[i].curent_stop].stop_frwd;
                    people_list[i].target_position.x += -16 + rand()%32;
                    people_list[i].target_position.y += -16 + rand()%32;
                }
                if(people_list[i].direction == 1)
                {
                    people_list[i].target_position = stops_list[people_list[i].curent_stop].stop_bkwd;
                    people_list[i].target_position.x += -16 + rand()%32;
                    people_list[i].target_position.y += -16 + rand()%32;
                }
            }
        }
        if(people_list[i].drive == 1)
        {
            people_list[i].position = taxi1.position;
            if(taxi1.wait && people_list[i].curent_stop != taxi1.curent_stop)
            {
                people_list[i].curent_stop = taxi1.curent_stop;
                int r = rand()%2;
                if(r == 1)
                {
                    pthread_mutex_lock(&hmtx);
                    people_list[i].drive = 0;
                    people_list[i].walk_out = 1;
                    taxi1.people_amount--;
                    people_list[i].target_position = stops_list[people_list[i].curent_stop].position;
                    people_list[i].target_position.x += -64 + rand()%128;
                    people_list[i].target_position.y += -64 + rand()%128;
                    pthread_mutex_unlock(&hmtx);
                }
            }
        }
        if(people_list[i].walk_out == 1)
        {
            coord dir_vector = {people_list[i].target_position.x-people_list[i].position.x, people_list[i].target_position.y-people_list[i].position.y};
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


            if(people_list[i].speed < 0.01)
            {
                people_list[i].walk_out = 0;
                people_list[i].sleep = 1;
                usleep(100000*(rand()%100 + 100));
                people_list[i].sleep = 0;
                people_list[i].walk_stop = 1;

                people_list[i].direction = rand()%2;
                if(people_list[i].direction == 0)
                {
                    people_list[i].target_position = stops_list[people_list[i].curent_stop].stop_frwd;
                    people_list[i].target_position.x += -16 + rand()%32;
                    people_list[i].target_position.y += -16 + rand()%32;
                }
                if(people_list[i].direction == 1)
                {
                    people_list[i].target_position = stops_list[people_list[i].curent_stop].stop_bkwd;
                    people_list[i].target_position.x += -16 + rand()%32;
                    people_list[i].target_position.y += -16 + rand()%32;
                }
            }
        }
        usleep(1000000/target_framerate);
    }
}


void showContent()
{
    XClearWindow(dspl, hwnd);
    
    XDrawRectangle(dspl, hwnd, gc, taxi1.position.x-8, taxi1.position.y - 4, 16, 8);
    char c[1] = "";
    sprintf(c, "%d", taxi1.people_amount);
    XDrawString(dspl, hwnd, gc, taxi1.position.x, taxi1.position.y, c, 1);
    
    coord op;
    for(int i = 0; i < 4; i++)
    {
        coord p1 = stops_list[i].stop_bkwd;
        XDrawRectangle(dspl, hwnd, gc, p1.x-stop_size/2, p1.y-stop_size/2, stop_size, stop_size);
        
        coord p2 = stops_list[i].stop_frwd;
        XDrawRectangle(dspl, hwnd, gc, p2.x-stop_size/2, p2.y-stop_size/2, stop_size, stop_size);

        coord p = stops_list[i].position;
        
        if(i > 0)
        {
            XDrawLine(dspl, hwnd, gc, op.x, op.y, p.x, p.y);
        }
        op = p;
    }
    char pc[4] = "";
    sprintf(pc, "%d", people_count);
   // XDrawString(dspl, hwnd, gc, 32, 32, pc, 4);

    for(int i = 0; i < people_count; i++)
    {
        XDrawRectangle(dspl, hwnd, gc, people_list[i].position.x - 2, people_list[i].position.y - 2, 4, 4);
        char c[1] = "";
        sprintf(c, "%d", i);
        XDrawString(dspl, hwnd, gc, 820, 16 + i * 18, c, 1);
        if(people_list[i].walk_stop == 1) XDrawString(dspl, hwnd, gc, 840, 16 + i * 18, "wk_st", 5);
        if(people_list[i].walk_taxi == 1) XDrawString(dspl, hwnd, gc, 880, 16 + i * 18, "wk_tx", 5);
        if(people_list[i].walk_out == 1) XDrawString(dspl, hwnd, gc, 920, 16 + i * 18, "wk_ot", 5);
        if(people_list[i].wait == 1) XDrawString(dspl, hwnd, gc, 960, 16 + i * 18, "wait", 4);
        if(people_list[i].drive == 1) XDrawString(dspl, hwnd, gc, 1000, 16 + i * 18, "drive", 5);
        if(people_list[i].direction == 1) XDrawString(dspl, hwnd, gc, 1060, 16 + i * 18, "bkwd", 4);
        if(people_list[i].direction == 0) XDrawString(dspl, hwnd, gc, 1100, 16 + i * 18, "frwd", 4);
    }

    XFlush(dspl);
}


void main()
{
    //strcpy(h, "Dijon");

    dspl = XOpenDisplay(NULL);//setting up new window
    gc = XDefaultGC(dspl,0);
    if(dspl == 0) {printf("Error XOpenDisplay\n"); exit(1);}
    screen = XDefaultScreen(dspl);
    hwnd = XCreateSimpleWindow(dspl, RootWindow(dspl, screen), 100,50,1200,600,3, BlackPixel(dspl,screen), WhitePixel(dspl,screen));
    if(hwnd == 0) {printf("Error XCreateSimpleWindow\n"); exit(1);}
    XSelectInput(dspl, hwnd, ExposureMask | KeyPressMask );//input mask
    XMapWindow(dspl, hwnd);

    
    int n = 0;
    char people_count_string[4] = "";
    while(n !=  -1)//inputs char from keyboard while it more then -1 and less then 10
    {
        //char buf[]= "yyy";
        XNextEvent(dspl, &event);
        if(event.type == KeyPress)
        {
            char kb[1];
            //XClearWindow(dspl,hwnd);
            XLookupString(&event.xkey, kb,1, &ks, 0);//converts keyKode to char symbol
            char c = kb[0];
            int i = c - '0';
            if(i >= 0 && i <=9)
            {
                XDrawString(dspl, hwnd, gc, 10 + n*10, 10, kb, 1);
                people_count_string[n] = kb[0];
                n++;
            }else{
                n = -1;
                sscanf(people_count_string, "%d", &people_count);
            }
        }
    }


    SetStops();


    for(int i = 0; i < people_count; i++)
    {
        people p;
        pthread_t thread;
        pthread_create(&thread, NULL, (void*)peopleThreadFunction, (void*)i);
    }




    taxi1.drive = 1;
    taxi1.direction = 1;
    taxi1.curent_stop = 1;
    taxi1.target_position = stops_list[0].position;
    while(1)
    {
        if(taxi1.drive == 1)
        {
            coord dir_vector = {taxi1.target_position.x-taxi1.position.x, taxi1.target_position.y-taxi1.position.y};
            float l = sqrt(pow(dir_vector.x,2)+pow(dir_vector.y, 2));
            coord normal_v  = dir_vector;
            normal_v.x /= l;
            normal_v.y /= l;
            if(taxi1.speed < 10 && taxi1.speed < l/20)
            {
                taxi1.speed += 0.05;
            }
            if(taxi1.speed > l/20)
            {
                taxi1.speed = l/20;
            }

            taxi1.position.x += normal_v.x * taxi1.speed;
            taxi1.position.y += normal_v.y * taxi1.speed;


            if(taxi1.speed < 0.001)
            {
                taxi1.drive = 0;
                taxi1.wait = 240;
                if(taxi1.direction == 0)
                {
                    taxi1.curent_stop++;
                    if(taxi1.curent_stop == 3) 
                    {
                        taxi1.direction = 1;
                    }
                }else{
                    taxi1.curent_stop--;
                    if(taxi1.curent_stop == 0) 
                    {  
                        taxi1.direction = 0;
                    }
                }      
            }
        }
        if(taxi1.wait > 0)
        {
            taxi1.wait--;
            if(taxi1.wait == 0) 
            { 
                taxi1.drive = 1;
                int c;
                if(taxi1.direction == 0)
                {
                    c = taxi1.curent_stop + 1;
                }
                if(taxi1.direction == 1)
                {
                    c = taxi1.curent_stop - 1;
                }
                taxi1.target_position = stops_list[c].position;
            }
        }
        showContent();
        usleep(1000000/target_framerate);
    }

    getchar(); printf("\033[0m");
}