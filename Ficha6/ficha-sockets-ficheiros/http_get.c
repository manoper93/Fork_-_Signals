#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int my_connect(char *servername, char *port);

int main (int argc, char* const argv[]) {

  char buffer[4096];
  char http_msg1[] = "GET /~jes/sistc/pl/ola.html HTTP/1.1\r\n"
    "Host: ave.dee.isep.ipp.pt\r\n\r\n";
    
  int s = my_connect("ave.dee.isep.ipp.pt", "80");
  
  //send HTTP request
  write(s, http_msg1, strlen(http_msg1));

  //receive HTTP server reply and print it on standard output
  int n;
  while( ( n = read(s, buffer, sizeof(buffer)-1) ) > 0 ) {
    buffer[n] = 0;
    fputs(buffer, stdout);
  }
  printf("\n\n");
  
  return 0;
}


int my_connect(char *servername, char *port) {

  //get server address using getaddrinfo
  struct addrinfo hints;
  struct addrinfo *addrs;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;
  //hints.ai_socktype = SOCK_STREAM;
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

