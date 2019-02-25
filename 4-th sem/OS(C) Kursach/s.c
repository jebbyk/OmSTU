//#define _WIN32_WINNT 0x0601
#include <ConsoleApi3.h>
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <string.h>

void main()
{
    short screenWidth = 192, screenHeight = 108, cs = 8;
   // COORD dwBufferCoord;
    SMALL_RECT writeRegion = { 0, 0, 1, 1 };

    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    HWND hWindow = GetConsoleWindow();

   // CONSOLE_SCREEN_BUFFER_INFO consoleBufInfo;
    CHAR_INFO *lpBuffer;
    //DWORD charectersWriten;
    
	SetConsoleWindowInfo(hStdOut, TRUE, &writeRegion);

	COORD bufferSize = { screenWidth, screenHeight };

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
	wcscpy_s(fontInfo.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(hStdOut, false, &fontInfo);

	SetConsoleMode(hStdOut, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
	//setting up window position and size
	SetWindowPos(hWindow, HWND_TOP, 0, 0, (int)w*cs + 16, (int)h*cs + 39, NULL);
	writeRegion = { 0, 0, (short)screenWidth - 1, (short)screenHeight - 1 };
	SetConsoleWindowInfo(hStdOut, TRUE, &writeRegion);

	lpBuffer = new CHAR_INFO[screenWidth * screenHeight]; // additional buffer for fasest draving;
	memset(lpBuffer, 0, sizeof(CHAR_INFO)*screenWidth*screenHeight);

	COORD cursorPos;
	cursorPos.X = 0;
	cursorPos.Y = 0;
	SetConsoleCursorPosition(hStdOut, cursorPos);



    CHAR_INFO IntanceTo18Levels(float _intance)
    {
        if (_intance > 1.0f) _intance = 1.0f;
        int intance = _intance * 18.0f;
        short bg_col, fg_col;
        wchar_t symbol;

        switch (intance)
        {
        case 0: bg_col = 0x0000; fg_col = 0x0000; symbol = 0x2588; break;

        case 1: bg_col = 0x0000; fg_col = 0x0008; symbol = '.'; break;
        case 2: bg_col = 0x0000; fg_col = 0x0008; symbol = ':'; break;
        case 3: bg_col = 0x0000; fg_col = 0x0008; symbol = '!'; break;

        case 4: bg_col = 0x0000; fg_col = 0x0008; symbol = 0x2591; break;

        case 5: bg_col = 0x0000; fg_col = 0x0008; symbol = 0x2573; break;//x
        case 6: bg_col = 0x0000; fg_col = 0x0008; symbol = '@'; break;

        case 7: bg_col = 0x0000; fg_col = 0x0008; symbol = 0x2592; break;
        case 8: bg_col = 0x0000; fg_col = 0x0008; symbol = 0x2593; break;
        case 9: bg_col = 0x0000; fg_col = 0x0008; symbol = 0x2588; break;

        case 10: bg_col = 0x0080; fg_col = 0x0007; symbol = 0x2591; break;
        case 11: bg_col = 0x0080; fg_col = 0x0007; symbol = 0x2592; break;
        case 12: bg_col = 0x0080; fg_col = 0x0007; symbol = 0x2593; break;
        case 13: bg_col = 0x0080; fg_col = 0x0007; symbol = 0x2588; break;

        case 14: bg_col = 0x0070; fg_col = 0x000f; symbol = 0x2591; break;
        case 15: bg_col = 0x0070; fg_col = 0x000f; symbol = 0x2592; break;
        case 16: bg_col = 0x0070; fg_col = 0x000f; symbol = 0x2593; break;
        case 17: bg_col = 0x0070; fg_col = 0x000f; symbol = 0x2588; break;

        case 18: bg_col = 0x0070; fg_col = 0x000f; symbol = 0x2588; break;
        //case 19: bg_col = 0x0070; fg_col = 0x000f; symbol = 0x2588; break;
        default: bg_col = 0x0000; fg_col = 0x0000; symbol = 0x2588;
        }
        CHAR_INFO c;
        c.Attributes = bg_col | fg_col;
        c.Char.UnicodeChar = symbol;
        return c;
    }

    void DrawPoint(short x, short y, float color)
    {
        //put a pixel in a buffer;
        if (x >= 0 && x < (short)screenWidth  && y >= 0 && y < (short)screenHeight)
        {
            CHAR_INFO  c = IntanceTo18Levels(color);
            lpBuffer[y*(short)screenWidth + x].Char.UnicodeChar = c;
            lpBuffer[y*screenWidth + x].Attributes = color;
        }
    }

    void ClearScreen()
    {
        for (int i = screenWidth * screenHeight; i > 0; i--)
        {
            lpBuffer[i].Char.UnicodeChar = ' ';// just fill buffer full of spaces;
            lpBuffer[i].Attributes = 0x000f;
        }
    }

    void PushBuffer()
    {
        WriteConsoleOutput(hStdOut, lpBuffer, { screenWidth, screenHeight }, { 0,0 }, &writeRegion);
    }   

    DrawPoint(10,10,1);
    PushBufer();
    _getch();

}