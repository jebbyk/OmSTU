#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void main()
{
    char *pb, st[20];//массив символв и уазатель на символ
    HANDLE hmem;//хендл общей памяти
    int k;
    HANDLE hmtx;//мутекс на эту память
    hmtx = OpenMutex(SYNCHRONIZE, TRUE,"myMutex");//открываем мьютекс

   if(hmtx != NULL )
   {
        hmem=OpenFileMapping(FILE_MAP_READ, FALSE, "SHAREMEM_MYY");//открываем память 
        
        if (hmem==0)//выводим ошибку если не получиллось
        {
            printf("Error OpenSharedMem with RC=%ld\n", GetLastError());
            getchar(); 
            exit(0);
        }
        pb=MapViewOfFile(hmem, FILE_MAP_READ,0,0,0);//берем разметку памяти
        printf("Mem addr %p\n", pb);//распечатываем полученный адерс
        if (pb==NULL)//есл ине получилось, печатаем ошибку
        {
            printf("Error Mapping SharedMem with RC=%ld\n", GetLastError());
            getchar(); 
            exit(0);
        }
        for (k=0;k<10;k++ )//десять раз повторояем
        {
            WaitForSingleObject(hmtx, INFINITE);//жедм разлока мьютекса
            strncpy(st, pb, 20); //пишем в память
            st[19]='\0'; //ишем конец файла
            printf("reading from file mapping: %s\n", st); //сообщеиня в консоль
            ReleaseMutex(hmtx);//отпукаем мьютекс
            Sleep(2000);//засыпаем

        }
        UnmapViewOfFile(pb); CloseHandle(hmem);//закрываем все
        
   }
}//досвидули, досвидули, досвидули