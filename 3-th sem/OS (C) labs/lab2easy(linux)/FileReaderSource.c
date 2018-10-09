#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

void main(){

    int fhandleOne, fhandleTwo, fhandleThree;
    char fileName[] = "output.txt";
    char buffer[7] = "";

    fhandleOne = open(fileName, O_RDONLY|O_CREAT, 0600);
    fhandleTwo = dup(fhandleOne);
    fhandleThree = open(fileName, O_RDONLY|O_CREAT, 0600);

    lseek(fhandleOne, 10, SEEK_SET);

    char number[3] = "";
    sprintf(number, "%d\n", fhandleOne);
    write(1, number, 3);
    sprintf(number, "%d\n", fhandleTwo);
    write(1, number, 3);
    sprintf(number, "%d\n", fhandleThree);
    write(1, number, 3);

    write(1, "\ntext from first handle\n",24);
    read(fhandleOne, buffer, 7);
    write(1, buffer, 7);

    write(1, "\ntext from seecond handle\n", 26);
    read(fhandleTwo, buffer, 7);
    write(1, buffer, 7);

    write(1, "\ntext from third handle\n", 24);
    read(fhandleThree, buffer, 7);
    write(1, buffer, 7);

    getchar();
    
    return ;
}