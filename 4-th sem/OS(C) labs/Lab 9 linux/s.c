#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>


char buf[12];
int hardnes = 1024, calcRes;


pthread_t rdrThrd1, rdrThrd2, rdrThrd3, wrtrThrd1, wrtrThrd2, wrtrThrd3;
pthread_mutex_t hmtx;

void procThreadWriter1(void *arg)
{
    char pat[12] = "abcdefghijkl";
    int k;
    for(k = 0; k < 24; k++)
    {
        pthread_mutex_lock(&hmtx);
        int i;
        for(i = 0; i < 6; i++)
        {
            buf[i] = pat[i];
        }
        pthread_mutex_unlock(&hmtx);

        usleep(1000000);

        pthread_mutex_lock(&hmtx);
        for(i = 6; i < 12; i++)
        {
            buf[i] = pat[i];
        }
        pthread_mutex_unlock(&hmtx);

        usleep(1000000);
    }
}


void procThreadWriter2(void *arg)
{
    char pat[12] = "ABCDEFGHIJKL";
    int k;
    for(k = 0; k < 24; k++)
    {
        pthread_mutex_lock(&hmtx);
        int i;
        for(i = 0; i < 6; i++)
        {
            buf[i] = pat[i];
        }
        pthread_mutex_unlock(&hmtx);

        usleep(1500000);

        pthread_mutex_lock(&hmtx);
        for(i = 6; i < 12; i++)
        {
            buf[i] = pat[i];
        }
        pthread_mutex_unlock(&hmtx);

        usleep(1500000);
    }
}

void procThreadWriter3(void *arg)
{
    char pat[12] = "-<+_!\%^&*\\|@";
    int k;
    for(k = 0; k < 24; k++)
    {
        pthread_mutex_lock(&hmtx);
        int i;
        for(i = 0; i < 6; i++)
        {
            buf[i] = pat[i];
        }
        pthread_mutex_unlock(&hmtx);

        usleep(2000000);

        pthread_mutex_lock(&hmtx);
        for(i = 6; i < 12; i++)
        {
            buf[i] = pat[i];
        }
        pthread_mutex_unlock(&hmtx);

        usleep(2000000);
    }
}

void procThreadReader1(void *arg)
{
    int j;
    for(j = 0; j < 40; j++)
    {
        pthread_mutex_lock(&hmtx);
        printf("\033[%d;0H\033[1;34m",j+1);
        int i;
        for(i = 0; i < 12; i++)
        {
            printf("%c", buf[i]);
        }
        printf("\n");
        pthread_mutex_unlock(&hmtx);
        usleep(150000);
    } 
}
void procThreadReader2(void *arg)
{
    int j;
    for(j = 0; j < 40; j++)
    {
         pthread_mutex_lock(&hmtx);
        printf("\033[%d;30H\033[1;34m",j+1);
        int i;
        for(i = 0; i < 12; i++)
        {
            printf("%c", buf[i]);
        }
        printf("\n");
        pthread_mutex_unlock(&hmtx);
        usleep(300000);
    } 
}
void procThreadReader3(void *arg)
{
    int j;
    for(j = 0; j < 40; j++)
    {
         pthread_mutex_lock(&hmtx);
        printf("\033[%d;60H\033[1;34m",j+1);
        int i;
        for(i = 0; i < 12; i++)
        {
            printf("%c", buf[i]);
        }
        printf("\n");
        pthread_mutex_unlock(&hmtx);
        usleep(600000);
    } 
}


void main()
{

    int rc;
    printf("\033[2J\n");//cls
    rc=pthread_create(&wrtrThrd1, NULL, (void*)procThreadWriter1, (void*)2);
    rc=pthread_create(&wrtrThrd2, NULL, (void*)procThreadWriter2, (void*)3);
    rc=pthread_create(&wrtrThrd3, NULL, (void*)procThreadWriter3, (void*)4);

    rc=pthread_create(&rdrThrd1, NULL, (void*)procThreadReader1, (void*)2);
    rc=pthread_create(&rdrThrd2, NULL, (void*)procThreadReader2, (void*)2);
    rc=pthread_create(&rdrThrd3, NULL, (void*)procThreadReader3, (void*)2);

 
    getchar(); printf("\033[0m");
}