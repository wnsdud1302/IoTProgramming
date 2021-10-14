#include <netdb.h>
#include<stdio.h>
struct hostent *gethostbyname(const char *hostname);
extern int h_errno;

int main(int argc, char *argv[]){
    struct hostent *host;
    host = gethostbyname(argv[1]); 
    if(host == NULL) {
        fprintf(stderr, "Cannot find IP address from %s\n", argv[1]);
        return 1; 
    }

    printf("Official name: %s\n", host->h_name);

    for(int i=0; host->h_aliases[i]; i++) {

    printf("Aliases %d: %s\n", i+1, host->h_aliases[i]);

    }
     printf("Address type: %s\n", (host->h_addrtype == PF_INET) ? "PF_INET":"Unknown");

    for(int i=0; host->h_addr_list[i]; i++) {

    printf("IP addr %d: %s\n", i+1, inet_ntoa(*(struct in_addr *)host->h_addr_list[i]));

    }
    return 0;
}


