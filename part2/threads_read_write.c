#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

//binary sempahores
sem_t mutex;
sem_t writeSem;
int rcount = 0;


void *write_to_pipe (void *arg);
void *read_from_pipe (void *arg);

int main()
{
    int i,b;
    int n = 3;
    pthread_t rtid[5],wtid[5];

    // initialization of binary semaphores
    sem_init(&mutex,0,1);
    sem_init(&writeSem,0,1);

    // creation of n threads with fuctions read and write as attibutes
    for(i=0;i<n;i++)
    {
        pthread_create(&wtid[i],NULL,write_to_pipe,(void *)(intptr_t)i);
        pthread_create(&rtid[i],NULL,read_from_pipe,(void *)(intptr_t)i);
    }

    // waits for the threads to terminates
    for(i=0;i<n;i++)
    {
        pthread_join(wtid[i],NULL);
        pthread_join(rtid[i],NULL);
    }

    // Delete the created file
    system("rm file.txt");

    return 0;
}

/* Read characters from the pipe and echo them to stdout. */

void *read_from_pipe (void *arg)
{
    FILE *stream;
    int c;
    int f;
    f = ((int)(intptr_t)arg);

    sem_wait(&mutex);

    // count for those who want to enter the read section
    rcount = rcount + 1;
    
    // if someone wants to read, then write waits
    if(rcount == 1)
        sem_wait(&writeSem);

    sem_post(&mutex);


    // Others who want to read can enter this section
    // read section (Start)
    
    printf("Reader %d reads:\n",f);
    stream = fopen ("file.txt", "r");

    while ((c = fgetc (stream)) != EOF)
    {
        putchar (c);
    }
        
    fclose (stream);
    
    // read section (End)

    sleep(1);
    sem_wait(&mutex);


    rcount = rcount - 1;

    // if noone wants to read, then write can enter the critical section
    if(rcount==0)
        sem_post(&writeSem);

    sem_post(&mutex);
}

/* Write some random text to the pipe. */
     
void *write_to_pipe (void *arg)
{
    FILE *stream;
    int f;
    f = ((int)(intptr_t)arg);

    // write enters its critical section if its allowed

    sem_wait(&writeSem);

    //write (Start)
    
    stream = fopen ("file.txt", "w");
        
    srand(time(NULL));
    int rnum = (rand()%1000) + 1;
        
    fprintf (stream, "Random number is %d.\n", rnum);
    printf("Writer %d writes to file.\n",f);
    
    fclose (stream);
    
    //write (End)

    sleep(1);
    sem_post(&writeSem);
}