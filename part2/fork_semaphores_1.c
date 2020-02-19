#include <stdio.h>          /* printf()                 */
#include <stdlib.h>         /* exit(), malloc(), free() */
#include <sys/types.h>      /* key_t, sem_t, pid_t      */
#include <sys/shm.h>        /* shmat(), IPC_RMID        */
#include <errno.h>          /* errno, ECHILD            */
#include <semaphore.h>      /* sem_open(), sem_destroy(), sem_wait().. */
#include <fcntl.h>          /* O_CREAT, O_EXEC          */
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

typedef sem_t      Semaphore;

Semaphore *s1;                 /*      synch semaphore      */ /*shared */
Semaphore *s2;                 /*      synch semaphore      */ /*shared */
Semaphore *s3;                 /*      synch semaphore      */ /*shared */
Semaphore *s4;                 /*      synch semaphore      */ /*shared */

int main ()
{
    int i;                        /* loop variables            */    
    pid_t pid;                    /* fork pid                  */
    int *p;                       /* shared variable           */ /*shared */
    unsigned int n = 5;           /* fork count                */
	
   
    /* initialize semaphores for shared processes */
    s1 = sem_open ("s1Sem", O_CREAT | O_EXCL, 0644, 0);
    s2 = sem_open ("s2Sem", O_CREAT | O_EXCL, 0644, 0);
    s3 = sem_open ("s3Sem", O_CREAT | O_EXCL, 0644, 0);
    s4 = sem_open ("s4Sem", O_CREAT | O_EXCL, 0644, 0);


    /* fork child processes */
    for (i = 0; i < n; i++)
    {
        pid = fork ();
     
	    if (pid < 0)
        {
        /* check for error      */
            sem_unlink ("s1Sem");   
            sem_close(s1);
            sem_unlink ("s2Sem");   
            sem_close(s2);
            sem_unlink ("s3Sem");   
            sem_close(s3);
            sem_unlink ("s4Sem");   
            sem_close(s4); 

            /* unlink prevents the semaphore existing forever */
            /* if a crash occurs during the execution         */
            printf ("Fork error.\n");
        }
        else if (pid == 0) /* child processes */
            break;                  
    }


    /******************************************************/
    /******************   PARENT PROCESS   ****************/
    /******************************************************/
    if (pid > 0)
	{
        /* wait for all children to exit */
        while (pid = waitpid (-1, NULL, 0))
        {
            if (errno == ECHILD)
                break;
        }


        /* unlink prevents the semaphore existing forever */
        /* if a crash occurs during the execution         */
        sem_unlink ("s1Sem");   
        sem_close(s1);
        sem_unlink ("s2Sem");   
        sem_close(s2);
        sem_unlink ("s3Sem");   
        sem_close(s3);
        sem_unlink ("s4Sem");   
        sem_close(s4); 
    }
    /******************************************************/
    /******************   CHILD PROCESS   *****************/
    /******************************************************/
    else
	{
        system("ls -l");
        sem_post (s1);
        sem_post (s2);

        sem_wait (s1);
        sem_wait (s2);
        system("ls -l");
        sem_post (s3);
        sem_post (s4);

        sem_wait (s3);
        system("ls -l");

        sem_wait (s4);
        system("ls -l");
    }

    exit (0);
}
