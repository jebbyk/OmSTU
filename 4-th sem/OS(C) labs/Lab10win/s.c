#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <stdio.h>

void main()
{


    if (CreateProcess("n.exe","child",NULL,NULL,TRUE,CREATE_NEW_CONSOLE,NULL,NULL,&cif,&pi)==TRUE)
    {

    }

}