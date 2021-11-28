#include "serv.h"
int main(int argc, char **argv) {
    int port, pid;
    int lsock, asock;

    struct sockaddr_in remote_sin;
    socklen_t remote_sin_len;

    if (argc < 2) {
        printf("Usage: \n");
        printf("%s {port}: runs HTTP server.\n", argv[0]);
        exit(0);
    }

    port = atoi(argv[1]);
    printf("server will listen to port: %d.\n", port);
    
    lsock = socket(AF_INET, SOCK_STREAM, 0);
    if (lsock < 0) {
        printf("failed to create lsock.\n");
        exit(1);
    }

    if (bind_lsock(lsock, port) < 0) {
        printf("failed to bind lsock.\n");
        exit(1);
    }

    if (listen(lsock, 10) < 0) {
        perror("failed to listen lsock.\n");
        exit(1);
    }

    // handle zombie process
    signal(SIGCHLD, SIG_IGN);

    while (1) {
        printf("waiting...\n");
        asock = accept(lsock, (struct sockaddr *)&remote_sin, &remote_sin_len);
        if (asock < 0) {
            printf("failed to accept.\n");
            continue;
        }

        pid = fork();
        if (pid == 0) { 
            close(lsock); http_handler(asock); close(asock);
            exit(0);
        }

        if (pid != 0)   { close(asock); }
        if (pid < 0)    { printf("failed to fork.\n"); }
    }
}

