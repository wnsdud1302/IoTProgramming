#include <stdio.h>
#include <netdb.h>
#include<arpa/inet.h>

int main(int argc, char *argv[]){
    struct hostent *host;
    struct sockaddr_in addr;

    inet_aton(argv[1], &addr.sin_addr);
    host = gethostbyaddr((char *)&addr.sin_addr, 4, PF_INET);
    if(host == NULL){
        fprintf(stderr, "Cannot find IP address from %s \n", argv[1]);
        return 1;
    }

    printf("Official name: %s \n", host->h_name);
    for(int i = 0; host->h_aliases[i]; i++){
        printf("Aliases %d: %s\n", i+1, host->h_aliases[i]);
    }
    printf("Address type: %s\n", (host->h_addrtype == PF_INET) ? : "PF_INET: Unkown");
    for(int i = 0; host->h_addr_list[i]; i++){
        printf("IP addr %d: %s\n", i+1, inet_ntoa(*(struct in_addr *) host->h_addr_list[i]));
    }

}