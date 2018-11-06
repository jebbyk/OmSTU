#include <stdio.h>
#include <windows.h>


void main()
{
    HANDLE fhandle, hstdout, hstdinput;
    DWORD len, actlen;
    BOOL rci;
    INPUT_RECORD  inputbuf;
  
    char buffer[128];
    char firstText[] = "Press enter to read sfile\n";
    char secondText[] = "Input read now is active!";
    char keyPressText[] = "\nPressed key is: ";
    char mouseButtonText[] = "\nmouse button is pressed" ;
    hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
    hstdinput = GetStdHandle(STD_INPUT_HANDLE);
    //BOOL e = SetConsoleMode(hstdinput, ENABLE_MOUSE_INPUT);
    system("CLS");
    WriteFile(hstdout, firstText, sizeof(firstText), &actlen, NULL);
    getchar();
    fhandle = CreateFile("input.txt", GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
    ReadFile(fhandle, buffer, 128, &len, 0);
    WriteFile(hstdout, buffer, len, &actlen, NULL);
    getchar();
    WriteFile(hstdout, secondText, sizeof(secondText), &actlen, NULL);

    while(1)
    {
        rci = ReadConsoleInput(hstdinput, &inputbuf, 1, &actlen);
        if (inputbuf.EventType == KEY_EVENT)
        {
            WriteFile(hstdout, keyPressText,sizeof(keyPressText), &actlen, NULL);
            char c[1];
            c[0] = inputbuf.Event.KeyEvent.uChar.AsciiChar;
            if(c != '\n')
            {
                WriteFile(hstdout, c, 1, &actlen, NULL);
            }
        }
        if(inputbuf.EventType == MOUSE_EVENT)
        {
            WriteFile(hstdout, mouseButtonText, sizeof(mouseButtonText), &actlen, NULL);
                
            /*COORD pos = inputbuf.Event.MouseEvent.dwMousePosition;
            int x = pos.X; int y = pos.Y;
            char c[4];
            itoa(x, c, 10);
            WriteFile(hstdout, " X: ", 4, &actlen, NULL);
            WriteFile(hstdout, c, 3, &actlen, NULL);
            itoa(y, c, 10);
            WriteFile(hstdout, " Y: ", 4, &actlen, NULL);
            WriteFile(hstdout, c, 3, &actlen, NULL);*/
        }
    }
}
