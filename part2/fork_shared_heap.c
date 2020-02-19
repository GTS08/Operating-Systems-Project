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

#define  ONE       1

typedef sem_t      Semaphore;

Semaphore *mutex;                 /*      synch semaphore      */ /*shared */

void swap(int *a, int *b);
void heapify(int arr[], int n, int i);
void heapSort(int arr[], int n);
void printArray(int arr[], int n);

int main ()
{
    int pos;                      /* priority position         */
    int i;                        /* loop variables            */
    key_t shmkey;                 /* shared memory key         */
    int shmid;                    /* shared memory id          */
    pid_t pid;                    /* fork pid                  */
    int *arr;                     /* shared array              */ /*shared */
    int n = 100;                  /* fork count                */
    unsigned int value;           /* semaphore value           */
	
	
    /* Initializes random number generator */								  
	srand(time(NULL));
								  
    /********************************************************/								       
    /* Initialize a shared variable in shared memory        */
    /* valid directory name and a number                    */
    shmkey = ftok ("/dev/null", 5);       
    shmid = shmget (shmkey, n*sizeof(int), 0644 | IPC_CREAT);

	/* shared memory error check */
    if (shmid < 0)                           
    {
        perror ("shmget\n");
        exit (1);
    }
    /********************************************************/								       




    /********************************************************/								       
	/* attach p to shared memory */
    arr  = (int *) shmat (shmid, NULL, 0);   

    /********************************************************/

    

    /********************************************************/								       



    /* initialize semaphores for shared processes */
    mutex = sem_open ("arrSem", O_CREAT | O_EXCL, 0644, ONE); 

    /********************************************************/

    /* fork child processes */
    for (i = 0; i < n; i++)
    {
        pid = fork ();
     
	    if (pid < 0) {
        /* check for error      */
            sem_unlink ("arrSem");   
            sem_close(mutex);  

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

        printf ("Parent: All children have exited.\n");
        
        printf("\nPriorities in heap: \n");
		printArray(arr, n);
		
        /* shared memory detach */
        shmdt (arr);
        shmctl (shmid, IPC_RMID, 0);

        /* unlink prevents the semaphore existing forever */
        /* if a crash occurs during the execution         */
        sem_unlink ("arrSem");   
        sem_close(mutex);  
    }
    /******************************************************/
    /******************   CHILD PROCESS   *****************/
    /******************************************************/
    else
	{
        sem_wait (mutex);           /* DOWN operation */

        printf ("***************************************\n");
		printf ("Child(%d) enters the critical section.\n", i);

        srand(time(NULL) ^ getpid());
        pos = (rand()%1000) + 1;

        int j = 0;
        while (arr[j]!=0)
        {
            j++;
        }
        arr[j] = pos;

        heapSort(arr, n);


        printf ("Child(%d) priority position is %d.\n", i, pos);
		printf ("Child(%d) exits the critical region.\n", i);
        printf ("***************************************\n");
        printf ("\n");
		
		
        sem_post (mutex);           /* UP operation */
    }

    exit (0);
}

void swap(int *a, int *b)
{
  int temp = *b;
  *b = *a;
  *a = temp;
}

void heapify(int arr[], int n, int i) 
{ 
    int smallest = i; 
    int l = 2 * i + 1;
    int r = 2 * i + 2;
  
    if (l < n && arr[l] > arr[smallest]) 
        smallest = l; 
  
    if (r < n && arr[r] > arr[smallest]) 
        smallest = r; 
  
    if (smallest != i) { 
        swap(&arr[i], &arr[smallest]); 
        heapify(arr, n, smallest); 
    } 
} 

void heapSort(int arr[], int n) 
{ 
    
    for (int i = n / 2 - 1; i >= 0; i--) 
        heapify(arr, n, i); 
  
    for (int i = n - 1; i >= 0; i--) { 
        swap(&arr[0], &arr[i]);  
        heapify(arr, i, 0); 
    } 
}

void printArray(int arr[], int n)
{
  for (int i = 0; i < n; ++i)
    printf("%d ", arr[i]);
  printf("\n");
}
