#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>

void main()
{
    Display *dspl;
    int screen;
    Window hwnd;
    XEvent event;
    KeySym ks;
    GC gc;
    int x, y,w,h;
    char buf[64];
    int count = 0; 

    dspl = XOpenDisplay(NULL);
    gc = XDefaultGC(dspl,0);
    if(dspl == 0) {printf("Error XOpenDisplay\n"); exit(1);}
    screen = XDefaultScreen(dspl);
    hwnd = XCreateSimpleWindow(dspl, RootWindow(dspl, screen), 100,50,1280,720,3, BlackPixel(dspl,screen), WhitePixel(dspl,screen));
    if(hwnd == 0) {printf("Error XCreateSimpleWindow\n"); exit(1);}
    XSelectInput(dspl, hwnd, ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask);//input mask
    XMapWindow(dspl, hwnd);
    while(1)
    {
        XNextEvent(dspl, &event);
        switch(event.type)
        {
            case KeyPress:
            {
                XLookupString(&event.xkey, buf,1, &ks, 0);//convert from key to char
                XClearWindow(dspl,hwnd);
                XDrawString(dspl, hwnd, gc, 10, 10,buf, 1);
                printf("%c KeyPressed\n", buf[0]); 
                break;
            }
            case ButtonPress:
            {
                x = event.xbutton.x;
                y = event.xbutton.y;
                count = snprintf(buf, 64,"%d Mouse Button Pressed in X:%d Y:%d",event.xbutton.button, x, y);
                XClearWindow(dspl,hwnd);
                XDrawString(dspl, hwnd, gc, x, y,buf, count);
                printf("%d Mouse Button Pressed in X:%d Y:%d\n", event.xbutton.button, x, y); 
                break;
            }
            case ConfigureNotify:
            {
                x = event.xconfigure.x;
                y = event.xconfigure.y;
                w = event.xconfigure.width;
                h = event.xconfigure.height;
                count = snprintf(buf, 64,"Resizing window (Position is X:%d Y:%d H:%d W:%d) ",x,y,h,w);
                XClearWindow(dspl,hwnd);
                XDrawString(dspl, hwnd, gc, 10, 10,buf, count);
                printf("Resizing window (Position is X:%d Y:%d H:%d W:%d) \n",x,y,h,w);
                break;
            }
        }
    }
}