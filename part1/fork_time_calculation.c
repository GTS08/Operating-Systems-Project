#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <time.h>

void foo();

int main() 
{
    int i = 0;
    int pid;
    int status;
    double avg;
    time_t start, end, dif;
      
    start = time(NULL);
    printf("Initial time = %ld sec\n", start);

    while(i != 10000)
    {
        pid = fork();
        
        if(pid == 0)
        { 
            foo();
            exit(0); 
        }
        else
        {
            waitpid(pid, &status, 0);
        }

        i++;
    }

    end = time(NULL);
    printf("Final time = %ld sec\n", end);

    dif = end - start;
    printf("Elapsed time = %ld sec\n", dif);

    avg = (double) dif / 10000;
    printf("Average time = %f sec\n", avg);

    return(0); 
} 

void foo()
{
    int x=0;

    x=x+10;
}