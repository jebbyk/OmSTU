#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

void main()
{
    int bites, previousBites;
    char buffer[128] = "Readed text: \n";// buffer with our text
    bites = 14;//14 simbols in a buffer now
    char control;
    int fileHandle;
    char fileName[] = "input.txt";

    fileHandle = open(fileName, O_RDONLY|O_CREAT|O_APPEND, 0600);
    if(fileHandle == -1) 
    {
        write(1, " error opening file \n", 20);
        return;
    }

    write(1, "1-read from file, 0-read from keyboard\n", 39);
   // read(0, controlBuffer, 1);
   control = getchar();
    if(control == '0')
    {
        write(1, "Input your text \n", 17); // print to std linux output
        // place pointer to end of text in buffer (buffer + 14) and read std input
        bites += read(0, buffer+bites, 128);//returns count of inputed simbols
        //write text from buffer to std output
        write(1,buffer, bites);
    }
    if(control == '1')
    {
        write(1, "Reading text from file: \n", 25);
        bites += read(fileHandle,buffer + bites,  128);
        write(1, buffer, bites);
    }
    getchar();
    return;
}