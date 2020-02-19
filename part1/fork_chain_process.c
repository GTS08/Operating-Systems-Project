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

        if(pid > 0) // if current process is the parent
        {   
            if (i != 0) // avoid printing the first parent
                printf("Parent id: %d, Process id: %d, Child id: %d\n", getppid(), getpid(), pid);
            wait(NULL); // wait for the child process to finish
            exit(0);
        }
        else
        {
            if(i == 9) // print status for the last child
                printf("Parent id: %d, Process id: %d, Child id: none\n", getppid(), getpid());
        }
    }

    return(0); 
} 
