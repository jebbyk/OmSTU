#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

void main()
{
    int bites, previousBites;
    char buffer[128] = "\nReaded text: ";// buffer with our text
    bites = 14;//14 simbols in a buffer now
    char controlBuffer[1];
    int fileHandle;
    char fileName[] = "input.txt";

    fileHandle = open(fileName, O_WRONLY|O_CREAT|O_APPEND, 0600);
    if(fileHandle == -1) 
    {
        write(1, "\n error opening file", 20);
        return;
    }

    write(1, "\n1-read from file, 0-read from keyboard", 39);
    read(0, controlBuffer, 1);
    if(controlBuffer[0] == '0')
    {
        write(1, "\nInput your text ", 17); // print to std linux output
        // place pointer to end of text in buffer (buffer + 14) and read std input
        bites += read(0, buffer+bites, 128);//returns count of inputed simbols
        //write text from buffer to std output
        write(1,buffer, bites);
    }
    if(controlBuffer[0] == '1')
    {
        write(1, "\nText from file: ", 17);
        bites += read(fileHandle,buffer + bites,  128);
        write(1, buffer, bites);
    }
    getchar();
    return;
}