#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h> 

int main()
{
    int pid1;
    int pid2;
    
    pid1 = fork();
    
    if(pid1 < 0)
        printf("Could not create any child\n");
    else
    {
        pid2 = fork();
        if(pid2 < 0)
            printf("Could not create any child\n");
        else
            if((pid1 < 0) && (pid2 < 0))
                kill(pid1,9);
    }

    if(pid2 == 0) // if current process is the child
    { 
        printf("Process id: %d Parent id: %d\n", getpid(), getppid());
        exit(0); 
    }
    else // if the current process is the parent
    {
        wait(NULL); // wait for the child process to finish
    }

    if(pid1 == 0) // if current process is the child
    { 
        printf("Process id: %d Parent id: %d\n", getpid(), getppid());
        exit(0); 
    }
    else // if the current process is the parent
    {
        printf("Process id: %d Parent id: %d\n", getpid(), getppid());
        wait(NULL); // wait for the child process to finish
    }

    sleep(20);

    return (0);
}
