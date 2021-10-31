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
 FILE * fp = fopen("test.html", "w"); //출력내용을 파일에 저장하기 위해
 struct hostent *remoteHost; //웹주소를 ip로 바꾸기 위해
 
 sock = socket(AF_INET, SOCK_STREAM, 0);
 //if (sock == INVALID_SOCKET)
 if (sock < 0)
  return 0;
  
 remoteHost = gethostbyname("google.com"); //웹주소를 ip로 바꾸기 위해
 
 //connect()
 //SOCKADDR_IN serveraddr;
 struct sockaddr_in client_addr;
 client_addr.sin_family = AF_INET;
 client_addr.sin_port = htons(80);
 client_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)remoteHost->h_addr_list[0]));
 retval = connect(sock, (struct sockaddr * ) &client_addr, sizeof(client_addr));
 printf("_>>>%s", inet_ntoa(*(struct in_addr*)remoteHost->h_addr_list[0]));
 //if (retval == SOCKET_ERROR)
 if (retval < 0)
 {
  printf("NOT connect!!!!!");
  return 0;
 }
 
 char msg[500] = "GET / HTTP/1.1\r\n";
 strcat(msg, "Host: www.google.com:80\r\n\r\n"); //웹서버로 보낼 헤더 작성

 send(sock, msg, strlen(msg), 0); //웹서버로 보내기
 while (1)
 {
  //if ((retval = read(sock, buf, BUFSIZE)) == 0)
  // break; //서버에서 응답 받기
  read(sock, buf, BUFSIZE);
  printf("%s", buf); //화면에 출력(너무 많아서 짤려서 보임)
  fprintf(fp, "%s", buf); //파일에 저장
  memset(buf, 0, BUFSIZE); //버퍼 지우기
 }
 close(sock);
 fclose(fp);
 //윈속 종료
 //WSACleanup();
 return 0;
}