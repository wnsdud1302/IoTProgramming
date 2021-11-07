#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>
 #include <stdlib.h>
 #include <stdio.h>
 #include <string.h>
 #define BUFSIZE 512
int main(int argc, char **argv)
{
 int sock;
 int retval;
 char buf[BUFSIZE];
 struct hostent *name;
 
 sock = socket(AF_INET, SOCK_STREAM, 0);
 if (sock < 0)
  return 0;

  name = gethostbyname(argv[1]); 

 struct sockaddr_in client_addr;
 client_addr.sin_family = AF_INET;
 client_addr.sin_port = htons(atoi(argv[2]));//argv[2]는 포트
 printf("%d\n", client_addr.sin_port);
 client_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)name->h_addr_list[0]));
 retval = connect(sock, (struct sockaddr * ) &client_addr, sizeof(client_addr));
 printf("_>>>%s\n", inet_ntoa(*(struct in_addr*)name->h_addr_list[0]));
 if (retval < 0)
 {
  printf("NOT connect!!!!!");
  return 0;
 }
 

 char msg[500] = "GET /webhp HTTP/1.1\r\nUser-Agent: Mozila/4.0\r\ncontent-type:text/html\r\nconnetion: close\r\n\r\n"; //웹서버로 보낼 헤더 작성

 send(sock, msg, strlen(msg), 0); //웹서버로 보내기
 while (1)
 {

  read(sock, buf, BUFSIZE);
  printf("%s", buf); //화면에 출력
  memset(buf, 0, BUFSIZE); //버퍼 지우기
 }
 close(sock);
 return 0;
}