#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h> 
  
int main() 
{
    int i;
    int pid;

    for(i=0; i<10; i++)
    {
        pid = fork(); // a child process is created

        if(pid == 0) // if current process is the child
        { 
            printf("Process id: %d Parent id: %d\n", getpid(), getppid()); 
            exit(0); 
        }
        else // if the current process is the parent
        {
            wait(NULL); // wait for the child process to finish
        }
    } 
    
    return(0); 
} 
