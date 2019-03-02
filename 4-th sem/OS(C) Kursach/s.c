#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <string.h>

void main()
{
    int w = 190, h = 106, cs = 10;
    
    COORD dwBufferCoord;
    dwBufferCoord.X = w;
    dwBufferCoord.Y = h;

    SMALL_RECT writeRegion = { 0, 0, w - 1, h - 1 };

    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    HWND hWindow = GetConsoleWindow();

    CHAR_INFO *lpBuffer;

	COORD bufferSize = { w, h };

	SetConsoleScreenBufferSize(hStdOut, bufferSize);//setting up char buffer of console
	SetConsoleActiveScreenBuffer(hStdOut);


	//setting up font
	CONSOLE_FONT_INFOEX fontInfo;
	fontInfo.cbSize = sizeof(fontInfo);
	fontInfo.nFont = 0;
	fontInfo.dwFontSize.X = cs;
	fontInfo.dwFontSize.Y = cs;
	fontInfo.FontFamily = FF_DECORATIVE;
	fontInfo.FontWeight = FW_NORMAL;
	wcscpy_s(fontInfo.FaceName, 16, L"Consolas");
	SetCurrentConsoleFontEx(hStdOut, FALSE, &fontInfo);

	SetConsoleMode(hStdOut, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
   
	//setting up window position and size
	SetWindowPos(hWindow, HWND_TOP, -4, -16, w*cs + 16, h*cs + 39, SWP_SHOWWINDOW);
 
	SetConsoleWindowInfo(hStdOut, TRUE, &writeRegion);

 

    lpBuffer = malloc(sizeof(CHAR_INFO)*w * h);
	memset(lpBuffer, 0, sizeof(CHAR_INFO)*w*h);

    CHAR_INFO IntanceTo18Levels(float _intance)//converts value between 0 and 1 to specified char + attribute color
    {
        if (_intance > 1.0f) _intance = 1.0f;
        int intance = _intance * 18.0f;
        short bg_col, fg_col;
        CHAR symbol;

        switch (intance)
        {
        case 0: bg_col = 0x0000; fg_col = 0x0000; symbol = 219; break;

        case 1: bg_col = 0x0000; fg_col = 0x0008; symbol = '.'; break;
        case 2: bg_col = 0x0000; fg_col = 0x0008; symbol = ':'; break;
        case 3: bg_col = 0x0000; fg_col = 0x0008; symbol = '!'; break;

        case 4: bg_col = 0x0000; fg_col = 0x0007; symbol = 176; break;

        case 5: bg_col = 0x0000; fg_col = 0x0008; symbol = 216; break;//x
        case 6: bg_col = 0x0000; fg_col = 0x0008; symbol = '@'; break;

        case 7: bg_col = 0x0000; fg_col = 0x0007; symbol = 177; break;
        case 8: bg_col = 0x0000; fg_col = 0x0008; symbol = 178; break;
        case 9: bg_col = 0x0000; fg_col = 0x0008; symbol = 219; break;

        case 10: bg_col = 0x0080; fg_col = 0x0007; symbol = 176; break;
        case 11: bg_col = 0x0080; fg_col = 0x0007; symbol = 177; break;
        case 12: bg_col = 0x0080; fg_col = 0x0007; symbol = 178; break;
        case 13: bg_col = 0x0080; fg_col = 0x0007; symbol = 219; break;

        case 14: bg_col = 0x0070; fg_col = 0x000f; symbol = 176; break;
        case 15: bg_col = 0x0070; fg_col = 0x000f; symbol = 177; break;
        case 16: bg_col = 0x0070; fg_col = 0x000f; symbol = 178; break;
        case 17: bg_col = 0x0070; fg_col = 0x000f; symbol = 219; break;

        case 18: bg_col = 0x0070; fg_col = 0x000f; symbol = 219; break;
        //case 19: bg_col = 0x0070; fg_col = 0x000f; symbol = 0x2588; break;
        default: bg_col = 0x0000; fg_col = 0x0000; symbol = 219;
        }
        CHAR_INFO c;
        c.Attributes = bg_col | fg_col;
        c.Char.AsciiChar = symbol;
        return c;
    }

    void DrawPoint(short x, short y, float color)//puts a pixell to a buffer
    {
        if (x >= 0 && x < (short)w  && y >= 0 && y < (short)h)
        {
            CHAR_INFO  c = IntanceTo18Levels(color);
            lpBuffer[y*(short)w + x].Char.AsciiChar = c.Char.AsciiChar;
            lpBuffer[y*w + x].Attributes = c.Attributes;
        }
    }

   void ClearScreen()// just fill buffer full of spaces;
    {
        for (int i = w * h; i > 0; i--)
        {
            lpBuffer[i].Char.AsciiChar = ' ';
            lpBuffer[i].Attributes = 0x000f;
        }
    }
    
    printf("\nsome magic2");
    COORD zCoord;
    zCoord.X = zCoord.Y = 0;
   void PushBuffer()
    {
        WriteConsoleOutput(hStdOut, lpBuffer, dwBufferCoord, zCoord, &writeRegion);
    }   

    float scale = 1;
    while(TRUE)// update loop
    {
        scale *= 0.997;
        ClearScreen();
        for(int y = 0; y < 106; y++)
        {
            for(int x = 0; x < 190; x++)
            {
                DrawPoint(x,y,(x*y)/(190.0f*106.0f*scale)); //test intancity gradient
            }
        }
        PushBuffer();
    }
}