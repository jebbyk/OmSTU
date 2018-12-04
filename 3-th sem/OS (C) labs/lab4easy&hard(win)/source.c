#include <stdio.h>
#include <windows.h>
#include <stdlib.h>


void main()
{
    HANDLE fhandle, hstdout, hstdinput;
    DWORD len, actlen;
    BOOL rci;
    INPUT_RECORD  inputbuf;
    CONSOLE_SCREEN_BUFFER_INFO ScreenBufInfo;
  
    char buffer[128];
    char firstText[] = "Press enter to read sfile\n";
    char secondText[] = "Input read now is active!";
    char keyPressText[] = "\nPressed key is: ";
    char mouseButtonText[] = "Mouse event detected" ;
    char lastText[] = "\nPress enter to end process";
    char errorText[] = "\nError setting console input mode";
    char error2Text[] = "\nError getting console input mode";
    hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
    hstdinput = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleScreenBufferInfo(hstdout, &ScreenBufInfo);
    WriteFile(hstdout, firstText, sizeof(firstText), &actlen, NULL);
    getchar();
    fhandle = CreateFile("input.txt", GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
    ReadFile(fhandle, buffer, 128, &len, 0);
    WriteFile(hstdout, buffer, len, &actlen, NULL);
    getchar();
    WriteFile(hstdout, secondText, sizeof(secondText), &actlen, NULL);

    SetConsoleMode(hstdinput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);
    while(1)
    {
        rci = ReadConsoleInput(hstdinput, &inputbuf, 1, &actlen);
        //printf("\n 5%d", inputbuf.EventType);
        //WriteFile(hstdout, inputbuf.EventType)
        if(inputbuf.EventType == MOUSE_EVENT)
        {
            if(inputbuf.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
            {
                GetConsoleScreenBufferInfo(hstdout, &ScreenBufInfo);
                SetConsoleCursorPosition(hstdout, inputbuf.Event.MouseEvent.dwMousePosition);
                WriteFile(hstdout, mouseButtonText, sizeof(mouseButtonText), &actlen, NULL);
                int x = inputbuf.Event.MouseEvent.dwMousePosition.X; 
                int y = inputbuf.Event.MouseEvent.dwMousePosition.Y;
                char c[4];
                itoa(x, c, 10);
                WriteFile(hstdout, " X: ", 4, &actlen, NULL);
                WriteFile(hstdout, c, 3, &actlen, NULL);
                itoa(y, c, 10);
                WriteFile(hstdout, " Y: ", 4, &actlen, NULL);
                WriteFile(hstdout, c, 3, &actlen, NULL);
            }
            if (inputbuf.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
            {
                system("cls");
                return;
            }
        }
        if (inputbuf.EventType == KEY_EVENT)
        {
           if(inputbuf.Event.KeyEvent.wVirtualKeyCode == VK_SHIFT){
                WriteFile(hstdout, keyPressText, sizeof(keyPressText), &actlen, NULL);
                WriteFile(hstdout, "shift", 5, &actlen, NULL);
            }

            if(inputbuf.Event.KeyEvent.wVirtualKeyCode == VK_TAB){
                WriteFile(hstdout, keyPressText, sizeof(keyPressText), &actlen, NULL);
                WriteFile(hstdout, "tab", 3, &actlen, NULL);
            }
            if(inputbuf.Event.KeyEvent.wVirtualKeyCode == VK_CONTROL){
                WriteFile(hstdout, keyPressText, sizeof(keyPressText), &actlen, NULL);
                WriteFile(hstdout, "ctrl", 4, &actlen, NULL);
            }
           WriteFile(hstdout, keyPressText,sizeof(keyPressText), &actlen, NULL);
            char c[1];
            c[0] = inputbuf.Event.KeyEvent.uChar.AsciiChar;
            WriteFile(hstdout, c, 1, &actlen, NULL);
        }
        
    }
    WriteFile(hstdout, lastText,sizeof(keyPressText), &actlen, NULL);
}
