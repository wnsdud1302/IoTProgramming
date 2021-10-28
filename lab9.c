#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>
#define _XOPEN_SOURCE 700

void read_childproc(int sig){
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if(WIFEXITED(status)){
        printf("REmoved proc id: %d \n", id);
        printf("child send: %d \n", WEXITSTATUS(status));
    }
}

int main(int argc, char *argv[]){
    pid_t pid;
    struct sigaction act;
    act.sa_handler=read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);

    pid = fork();
    if(pid==0){
        puts("HI i'm child process");
        sleep(12);
        return 12;
    }
    else{
        printf("chiled proc id: %d \n", pid);
        pid = fork();
        if(pid==0){
            puts("HI i'm chiled process");
            sleep(10);
            exit(24);
        }
        else{
            int i;
            printf("child proc id : %d \n", pid);
            for(i = 0; i < 5; i++){
                puts("wait...");
                sleep(5);
            }
        }
    }
    return 0;
}
