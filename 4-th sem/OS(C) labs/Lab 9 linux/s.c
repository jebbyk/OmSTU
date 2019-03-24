#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <X11/Xlib.h>


char buf[12];
int hardnes = 1024, calcRes;

Display *dspl;
int screen;
Window hwnd;
XEvent event;
KeySym ks;
GC gc;
int x, y,w,h;
int count = 0;
int speed = 100000;




pthread_t rdrThrd1, rdrThrd2, rdrThrd3, wrtrThrd1, wrtrThrd2, wrtrThrd3;
pthread_mutex_t hmtx;

void procThreadWriter1(void *arg)
{
    char pat[12] = "abcdefghijkl";
    int k;
    for(k = 0; k < 32; k++)
    {
        pthread_mutex_lock(&hmtx);
        int i;
        for(i = 0; i < 6; i++)
        {
            buf[i] = pat[i];
        }
        pthread_mutex_unlock(&hmtx);

        usleep(1*speed);

        pthread_mutex_lock(&hmtx);
        for(i = 6; i < 12; i++)
        {
            buf[i] = pat[i];
        }
        pthread_mutex_unlock(&hmtx);

        usleep(1*speed);
    }
}


void procThreadWriter2(void *arg)
{
    char pat[12] = "ABCDEFGHIJKL";
    int k;
    for(k = 0; k < 32; k++)
    {
        pthread_mutex_lock(&hmtx);
        int i;
        for(i = 0; i < 6; i++)
        {
            buf[i] = pat[i];
        }
        pthread_mutex_unlock(&hmtx);

        usleep(2*speed);

        pthread_mutex_lock(&hmtx);
        for(i = 6; i < 12; i++)
        {
            buf[i] = pat[i];
        }
        pthread_mutex_unlock(&hmtx);

        usleep(2*speed);
    }
}

void procThreadWriter3(void *arg)
{
    char pat[12] = "-<+_!\%^&*\\|@";
    int k;
    for(k = 0; k < 32; k++)
    {
        pthread_mutex_lock(&hmtx);
        int i;
        for(i = 0; i < 6; i++)
        {
            buf[i] = pat[i];
        }
        pthread_mutex_unlock(&hmtx);

        usleep(3*speed);

        pthread_mutex_lock(&hmtx);
        for(i = 6; i < 12; i++)
        {
            buf[i] = pat[i];
        }
        pthread_mutex_unlock(&hmtx);

        usleep(3*speed);
    }
}

void procThreadReader1(void *arg)
{
    int j;
    for(j = 0; j < 32; j++)
    {
        pthread_mutex_lock(&hmtx);
        printf("\033[%d;0H\033[1;34m",j+1);
        int i;
        for(i = 0; i < 12; i++)
        {
            printf("%c", buf[i]);
        }
        printf("\n");

        XDrawString(dspl, hwnd, gc, 20*8, j*16,buf, 12);
        XFlush(dspl);

        pthread_mutex_unlock(&hmtx);
        usleep(2*speed);
    } 
}
void procThreadReader2(void *arg)
{
    int j;
    for(j = 0; j < 32; j++)
    {
         pthread_mutex_lock(&hmtx);
        printf("\033[%d;30H\033[1;34m",j+1);
        int i;
        for(i = 0; i < 12; i++)
        {
            printf("%c", buf[i]);
        }
        printf("\n");

        XDrawString(dspl, hwnd, gc, 40*8, j*16,buf, 12);
        XFlush(dspl);

        pthread_mutex_unlock(&hmtx);
        usleep(4*speed);
    } 
}
void procThreadReader3(void *arg)
{
    int j;
    for(j = 0; j < 32; j++)
    {
         pthread_mutex_lock(&hmtx);
        printf("\033[%d;60H\033[1;34m",j+1);
        int i;
        for(i = 0; i < 12; i++)
        {
            printf("%c", buf[i]);
        }
        printf("\n");

        XDrawString(dspl, hwnd, gc, 60*8, j*16,buf, 12);
        XFlush(dspl);

        pthread_mutex_unlock(&hmtx);
        usleep(6*speed);
    } 
}


void main()
{

    dspl = XOpenDisplay(NULL);
    gc = XDefaultGC(dspl,0);
    if(dspl == 0) {printf("Error XOpenDisplay\n"); exit(1);}
    screen = XDefaultScreen(dspl);
    hwnd = XCreateSimpleWindow(dspl, RootWindow(dspl, screen), 100,50,1280,720,3, BlackPixel(dspl,screen), WhitePixel(dspl,screen));
    if(hwnd == 0) {printf("Error XCreateSimpleWindow\n"); exit(1);}
    XSelectInput(dspl, hwnd, ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask);//input mask
    XMapWindow(dspl, hwnd);

    int rc;
    printf("\033[2J\n");//cls
    rc=pthread_create(&wrtrThrd1, NULL, (void*)procThreadWriter1, (void*)2);
    rc=pthread_create(&wrtrThrd2, NULL, (void*)procThreadWriter2, (void*)3);
    rc=pthread_create(&wrtrThrd3, NULL, (void*)procThreadWriter3, (void*)4);

    rc=pthread_create(&rdrThrd1, NULL, (void*)procThreadReader1, (void*)2);
    rc=pthread_create(&rdrThrd2, NULL, (void*)procThreadReader2, (void*)2);
    rc=pthread_create(&rdrThrd3, NULL, (void*)procThreadReader3, (void*)2);

    getchar(); printf("\033[0m");
}