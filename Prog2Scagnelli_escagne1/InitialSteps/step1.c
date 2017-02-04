#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h> 
#include <unistd.h>


int main(){

    pid_t pid; /* for a child process */
    
    pid = fork();
    if(pid < 0){ //Error
        fprintf(stderr, "Fork failed.\n");
        return 1;
    }

    if(pid == 0){ //Child process  
        execlp("/bin/ls", "ls", NULL);
        sleep(5);
    }
    else{ //Parent process
        wait(NULL);
        printf("Child complete.\n");
    }
    return 0;
}
