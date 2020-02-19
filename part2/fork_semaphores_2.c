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

Semaphore *s12;                 /*      synch semaphore      */ /*shared */
Semaphore *s13;                 /*      synch semaphore      */ /*shared */
Semaphore *s24;                 /*      synch semaphore      */ /*shared */
Semaphore *s25;                 /*      synch semaphore      */ /*shared */
Semaphore *s35;                 /*      synch semaphore      */ /*shared */
Semaphore *s36;                 /*      synch semaphore      */ /*shared */
Semaphore *s46;                 /*      synch semaphore      */ /*shared */

int main ()
{
    int i;                        /* loop variables            */    
    pid_t pid;                    /* fork pid                  */
    int *p;                       /* shared variable           */ /*shared */
    unsigned int n = 5;           /* fork count                */
	
   
    /* initialize semaphores for shared processes */
    s12 = sem_open ("s12Sem", O_CREAT | O_EXCL, 0644, 0);
    s13 = sem_open ("s13Sem", O_CREAT | O_EXCL, 0644, 0);
    s24 = sem_open ("s24Sem", O_CREAT | O_EXCL, 0644, 0);
    s25 = sem_open ("s25Sem", O_CREAT | O_EXCL, 0644, 0);
    s35 = sem_open ("s35Sem", O_CREAT | O_EXCL, 0644, 0);
    s36 = sem_open ("s36Sem", O_CREAT | O_EXCL, 0644, 0);
    s46 = sem_open ("s46Sem", O_CREAT | O_EXCL, 0644, 0);


    /* fork child processes */
    for (i = 0; i < n; i++)
    {
        pid = fork ();
     
	    if (pid < 0)
        {
        /* check for error      */
            sem_unlink ("s12Sem");
            sem_close(s12);
            sem_unlink ("s13Sem");
            sem_close(s13);
            sem_unlink ("s24Sem");
            sem_close(s24);
            sem_unlink ("s25Sem");
            sem_close(s25);
            sem_unlink ("s35Sem");
            sem_close(s35);
            sem_unlink ("s36Sem");
            sem_close(s36);
            sem_unlink ("s46Sem");
            sem_close(s46);

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
        system("ls -l");
        sem_post (s12);
        sem_post (s13);

        sem_wait (s12);
        system("ls -l");
        sem_post (s24);
        sem_post (s25);

        sem_wait (s13);
        system("ls -l");
        sem_post (s35);
        sem_post (s36);

        sem_wait (s24);
        system("ls -l");
        sem_post (s46);

        sem_wait (s25);
        sem_wait (s35);
        system("ls -l");

        sem_wait (s46);
        sem_wait (s36);
        system("ls -l");

        /* wait for all children to exit */
        while (pid = waitpid (-1, NULL, 0))
        {
            if (errno == ECHILD)
                break;
        }


        /* unlink prevents the semaphore existing forever */
        /* if a crash occurs during the execution         */
        sem_unlink ("s12Sem");
        sem_close(s12);
        sem_unlink ("s13Sem");
        sem_close(s13);
        sem_unlink ("s24Sem");
        sem_close(s24);
        sem_unlink ("s25Sem");
        sem_close(s25);
        sem_unlink ("s35Sem");
        sem_close(s35);
        sem_unlink ("s36Sem");
        sem_close(s36);
        sem_unlink ("s46Sem");
        sem_close(s46);
    }
    /******************************************************/
    /******************   CHILD PROCESS   *****************/
    /******************************************************/
    else
	{
        system("ls -l");
        sem_post (s12);
        sem_post (s13);

        sem_wait (s12);
        system("ls -l");
        sem_post (s24);
        sem_post (s25);

        sem_wait (s13);
        system("ls -l");
        sem_post (s35);
        sem_post (s36);

        sem_wait (s24);
        system("ls -l");
        sem_post (s46);

        sem_wait (s25);
        sem_wait (s35);
        system("ls -l");

        sem_wait (s46);
        sem_wait (s36);
        system("ls -l");
    }

    exit (0);
}
