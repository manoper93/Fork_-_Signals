#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


void recv_server_reply1(int);
void recv_server_reply2(int);
int my_connect(char *servername, char *port);
void print_socket_address(int sd);

int main (int argc, char* const argv[]) {

  if(argc!=3) {
    printf("Usage: %s server_ip_address port_number\n",argv[0]);
    exit(1); 
  }
    
  //read a string
  char buffer[4096];  
  printf("Enter a string: ");
  fgets(buffer, sizeof(buffer), stdin);
  
  
  //connect to server
  int socket_descriptor = my_connect(argv[1], argv[2]);
  
  //print local address
  print_socket_address(socket_descriptor);

  
  //send string to server
  write(socket_descriptor, buffer, strlen(buffer));
  

  //Receive server reply. Check the difference between both versions of the routine.
  recv_server_reply1(socket_descriptor);
  //recv_server_reply2(socket_descriptor); 
  
  return 0;
}

int my_connect(char *servername, char *port) {

  //get server address using getaddrinfo
  struct addrinfo hints;
  struct addrinfo *addrs;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;
  int r = getaddrinfo(servername, port, &hints, &addrs);
  if (r != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(r));
    exit(1);
  }
  
  //create socket
  int s = socket(AF_INET, SOCK_STREAM, 0);
  if (s == -1) {
    perror("socket");  
    exit(2);
  }
  
  //connect to server
  r = connect(s, addrs->ai_addr, addrs->ai_addrlen);
  if (r == -1) {
    perror("connect");  
    close(s);
    exit(3);
  }

  freeaddrinfo(addrs);  
  return s;
}


 //version 1 - prints server reply as it receives it (by parts)
void recv_server_reply1(int s) 
{
  char buf[4096];

  printf("Reply from server: ");
  while(1) 
  {
    int n = read(s, buf, sizeof(buf)-1);
    if(n==0)
      break;
    buf[n] = 0;//terminate string (read does not do this)    
    printf(buf);
    fflush(stdout);
  }
  
  printf("\n\n");
}

//version 2 - receives whole answer and then prints it  
void recv_server_reply2(int s) 
{
  char buf[4096];
  
  int bytes_recv = 0;
  while(1) 
  {
    int n = read(s, buf + bytes_recv, sizeof(buf) - 1 - bytes_recv);
    if(n==0)
      break;

    bytes_recv += n;
  }
  
  buf[bytes_recv] = 0;//terminate string (read does not do this)
  printf("Reply from server: %s\n\n", buf);
}

void print_socket_address(int sd)
{
  struct sockaddr_in addr;
  socklen_t addrlen = sizeof(addr);

  getsockname(sd, (struct sockaddr *) &addr, &addrlen);

  char hostname[256];  
  char port[6];

  int n = getnameinfo((struct sockaddr *) &addr, addrlen, hostname, sizeof(hostname), 
    port, sizeof(port), NI_NUMERICHOST | NI_NUMERICSERV); 
  if(n != 0)
    printf("%s\n", gai_strerror(n));
  else
    printf("Socket address: %s:%s\n", hostname, port);  
}

/*
 * 1.a.1)
addr e porto

 * 1.a.2)
buffer

 * 1.a.3)
echo protocol

* 1.c)
* ./srv_exemplo 1024

Connection from 127.0.0.1:40096
Waiting connection

* 1.d)
* ./clt_exemplo 127.0.0.1 1024

Enter a string: asdasd
Socket address: 127.0.0.1:40096
Reply from server: ASDASD


 */
