#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h> 
#include <unistd.h>
#include <stdbool.h>


int main(){

    pid_t pid; /* for a child process */

    pid = fork();
    if(pid < 0){ //Error
        fprintf(stderr, "Fork failed.\n");
        return 1;
    }
    if(pid == 0){ //First child code
        pid = fork();
        if(pid < 0){ //Error
            fprintf(stderr, "Fork failed.\n");
            return 1;
        }
        if(pid == 0){ //grandchild code
            printf("I am the grandchild. My pid is %d and my parents pid is %d\n",getpid(),getppid());
            execlp("/bin/ls", "ls", NULL);         
        }
        else{
            wait(NULL);
            execlp("/import/linux/home/escagne1/Desktop/Operating-Systems/Prog2Scagnelli_escagne1/suspendResume", "suspendResume", NULL);
        }
    }
    else{
        wait(NULL);
        for(int i = 0; i < 3; i++){
            pid = fork();
            if(pid < 0){ //Error
                fprintf(stderr, "Fork failed.\n");
                return 1;
            }

            if(pid == 0){ //Child process  
                execlp("/import/linux/home/escagne1/Desktop/Operating-Systems/Prog2Scagnelli_escagne1/suspendResume", "suspendResume", NULL);
            }
            else{ //Parent process
                wait(NULL);
                printf("Child complete.\n");
            }
        }
    }
    return 0;
}
