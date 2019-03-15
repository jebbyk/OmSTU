#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#define _SW 256
#define _SH 144


void main()
{
    char charGradient[] = {' ', '.',':',';','~','"','!','|','+','*','#','@',176,177,178,219};
    char screenBuffer[_SH*_SW] = " ";
    float w = _SW, h = _SH;
    short targetFrameRate = 60;
   
    int intance = 0;
    char intanceToChar(float _intance)
    {
        if(_intance > 1.0f) _intance = 1.0f;
        else {if(_intance < 0.0f) _intance = 0.0f;}
        intance = _intance * 15;
        return charGradient[intance];
    }

    void drawPoint(int x, int y, float color)
    {
        screenBuffer[y*_SW+x] = intanceToChar(color);
    }

    void clearScreen()
    {
        /*char str[1000];
        memset(str, '\n', 999);
        str[999] = 0;
        write(1, str, 1000);*/
        memset(screenBuffer, ' ', w*h);
    }

    float scale = 0;

    while(1)
    {
        scale +=  1.0f / targetFrameRate;
        float s = sin(scale);
        int bites = 0;
       
        clearScreen();

       //debugging color gradient and flickering
        for(int y = 0; y < h; y++)
        {
            for(int x = 0; x < w; x++)
            {
                drawPoint(x,y,1.1f-(x*y)/(h*w)*(s+3.0f));
            }
        }
        write(1, screenBuffer, w*h);
        usleep(1000000/targetFrameRate);
    }
}