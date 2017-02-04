#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


int main(int argc, char *argv[]){

    char arg = *argv[1];
    if(arg == '0'){ //First child is the caller - create grandchild
        pid_t pid;
        pid = fork();
        if(pid < 0){
            fprintf(stderr, "Fork Failure\n");
            return 1;
        }
        if(pid == 0){//Child process
            printf("I am the grandchild. My PID is %d and my parents pid is %d\n", getpid(), getppid());
            execlp("/import/linux/home/escagne1/Desktop/Operating-Systems/Prog2Scagnelli_escagne1/InitialSteps/suspendResume", "suspendResume", "1", NULL);
        }
        else{//Parent process
            wait(NULL);
            printf("process id is %d and parent id is %d\n", getpid(), getppid());
            sleep(2);
            printf("After sleep\n");
        }

    }
    else{
        printf("process id is %d and parent id is %d\n", getpid(), getppid());
        sleep(2);
        printf("After sleep\n");
    }
    return 0;

}
