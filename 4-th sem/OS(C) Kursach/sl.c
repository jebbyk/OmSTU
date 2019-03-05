#include <stdio.h>
#include <unistd.h>
#include <math.h>
#define _SW 192
#define _SH 108


void main()
{
  

    char charGradient[] = {' ', '.',';','!','+','*','#','@',176,177,178,219};

    char screenBuffer[_SH*_SW] = " ";
    char intanceToChar(float _intance)
    {
        if(_intance > 1.0f) _intance = 1.0f;
        if(_intance < 0.0f) _intance = 0.0f;
        int intance = _intance * 11
        ;
        return charGradient[intance];
    }

    void clearScreen()
    {
        printf("\033[2J\033[1;1H");
        //printf("\033c");
        printf("\033[^l");
    }


    float scale = 0;
    while(1)
    {
        scale +=  0.002;
        float s = sin(scale);
        int bites = 0;
       
        clearScreen();


       float w = _SW, h = _SH;
        for(int y = 0; y < h; y++)
        {
            for(int x = 0; x < w; x++)
            {
                screenBuffer[y*_SW+x] = intanceToChar((x*y)/(h*w)*(s+2.0f));
                bites++;
            }
        }
        write(1, screenBuffer, bites);
        usleep(1000000/60);
    }
}