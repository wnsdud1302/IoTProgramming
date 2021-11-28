#define SERV_H
#ifdef SERV_H

#define BUF_SIZE 1000
#define HEADER "HTTP/1.1 %d %s\nContent-Length: %ld\nContent-Type: %s\n\n"

#define NOTFOUND       "<h1>404 Not Found</h1>\n"
#define SEVERERROR      "<h1>500 Internal Server Error</h1>\n"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>




int bind_lsock(int lsock, int port) {
    struct sockaddr_in sin;

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(port);

    return bind(lsock, (struct sockaddr *)&sin, sizeof(sin));
}


void fill_header(char *header, int status, char *type, long len) {
    char st_txt[40];
    switch (status) {
        case 200:
            strcpy(st_txt, "OK"); break;
        case 404:
            strcpy(st_txt, "Not Found"); break;
        case 500:
        default:
            strcpy(st_txt, "Internal Server Error"); break;
    }
    sprintf(header, HEADER, status, st_txt, len, type);
}


void find_type(char *f_type, char *uri) {
    char *ext = strrchr(uri, '.');
    if (!strcmp(ext, ".html")) 
        strcpy(f_type, "text/html");
    else if (!strcmp(ext, ".jpg") || !strcmp(ext, ".jpeg")) 
        strcpy(f_type, "image/jpeg");
    else if (!strcmp(ext, ".png"))
        strcpy(f_type, "image/png");
    else if (!strcmp(ext, ".css"))
        strcpy(f_type, "text/css");
    else if (!strcmp(ext, ".js"))
        strcpy(f_type, "text/javascript");
    else strcpy(f_type, "text/plain");
}
void error_handler(int asock, int type){
    char header[BUF_SIZE];
    if (type == 404){
        fill_header(header, 404, "text/html", sizeof(NOTFOUND));
        write(asock, header, strlen(header));
        write(asock, NOTFOUND, sizeof(NOTFOUND));
    }
    if(type == 500){
        fill_header(header, 500, "text/html", sizeof(SEVERERROR));
        write(asock, header, strlen(header));
        write(asock, SEVERERROR, sizeof(SEVERERROR));
    }
}
/*
    @func main http handler; try to open and send requested resource, calls error handler on failure
    @return
*/
void http_handler(int asock) {
    char header[BUF_SIZE];
    char buf[BUF_SIZE];

    if (read(asock, buf, BUF_SIZE) < 0) {
        printf("Failed to read request.\n");
        error_handler(asock, 500);
        return;
    }

    char *method = strtok(buf, " ");
    char *uri = strtok(NULL, " ");
    if (method == NULL || uri == NULL) {
        printf("Failed to identify method, URI.\n");
        error_handler(asock, 500);
        return;
    }

    printf("Request: method=%s, URI=%s\n", method, uri);
    
    char safe_uri[BUF_SIZE];
    char *local_uri;
    struct stat st;

    strcpy(safe_uri, uri);
    if (!strcmp(safe_uri, "/")) strcpy(safe_uri, "/index.html");
    
    local_uri = safe_uri + 1;
    if (stat(local_uri, &st) < 0) {
        printf("[WARN] No file found matching URI.\n");
        error_handler(asock, 404);
        return;
    }

    int fd = open(local_uri, O_RDONLY);
    if (fd < 0) {
        printf("Failed to open file.\n");
        error_handler(asock, 500);
        return;
    }

    int ct_len = st.st_size;
    char f_type[40];
    find_type(f_type, local_uri);
    fill_header(header, 200, f_type, ct_len);
    write(asock, header, strlen(header));

    int cnt;
    while ((cnt = read(fd, buf, BUF_SIZE)) > 0)
        write(asock, buf, cnt);
}

#endif