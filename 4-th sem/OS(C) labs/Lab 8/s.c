#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
char lbuk[ ]="abcdefghijklmnoprstvuwxy";
pthread_t tid1, tid2, tid3;
pthread_mutex_t hmtx;

void procthread1(void *arg)
{
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
    int k, j;
    for (k=0; k<24; k++)
    {
        pthread_mutex_lock(&hmtx);
        printf("\033[%d;20H\033[1;34m",k+1);
        for (j=0; j< 2*(int)arg; j++) printf("%c",lbuk[k]);
        printf("\n");
        pthread_mutex_unlock(&hmtx);

        usleep(1000000);
    }
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
}

void procthread2(void *arg)
{
    int k, j;
    for (k=0; k<24; k++)
    {
        pthread_mutex_lock(&hmtx);
        printf("\033[%d;40H\033[1;32m",k+1);
        for (j=0; j < 2 * (int)arg; j++) printf("%c",lbuk[k]);
        printf("\n");
        pthread_mutex_unlock(&hmtx);

        usleep(1000000);
    }
}
void procthread3(void *arg)
{   
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
    int k, j;
    for (k=0; k<24; k++)
    {
       if(k == 16) pthread_testcancel();
       if(k == 17) pthread_testcancel();
        
        pthread_mutex_lock(&hmtx);
        printf("\033[%d;60H\033[31m",k+1);
        for (j=0; j < 2 * (int)arg; j++) printf("%c",lbuk[k]);
        printf("\n");
        pthread_mutex_unlock(&hmtx);

        if (k == 13) 
        {
            pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
            pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
        }

        usleep(1000000);
    }
}

void main()
{
    int k;
    int rc;
    printf("\033[2J\n");
    pthread_mutex_init(&hmtx, NULL);
    rc=pthread_create(&tid1, NULL, (void*)procthread1, (void*)2);
    rc=pthread_create(&tid2, NULL, (void*)procthread2, (void*)3);
    rc=pthread_create(&tid3, NULL, (void*)procthread3, (void*)4);
    for (k=0; k<24; k++)
    {
        if( k == 6) {pthread_cancel(tid1); printf("    try cancel 1-st\n");}
        if( k == 11) {pthread_cancel(tid3); printf("    try cancel 2-nd\n");}

        pthread_mutex_lock(&hmtx);
        printf("\033[%d;1H\033[37m",k+1);
        printf("%c++\n",lbuk[k]);
        pthread_mutex_unlock(&hmtx);

        usleep(1000000);
    }
    getchar(); printf("\033[0m");
}