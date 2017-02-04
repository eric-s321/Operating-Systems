#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h> 
#include <unistd.h>


int main(){

    pid_t pid; /* for a child process */
    
    for(int i = 0; i < 4; i++){

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
    return 0;
}
