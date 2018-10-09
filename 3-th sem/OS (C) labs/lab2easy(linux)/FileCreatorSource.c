#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>


void main()
{
    int fileHandle;
    char fileName[] = "output.txt";
    char buffer[128] = "";
    int bites = 0;
    //open for writing and clear before writing (truncate)
    fileHandle = open(fileName, O_WRONLY|O_CREAT|O_TRUNC, 0600);
    write(1, "Input your text\n", 16);
    bites += read(0, buffer, 128);//read from keyboard(0) to buffer
    write(fileHandle, buffer, bites);//write to file from buffer
    getchar();
    return;
}