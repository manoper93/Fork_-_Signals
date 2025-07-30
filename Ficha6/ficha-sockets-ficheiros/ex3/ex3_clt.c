#include <unistd.h> 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>

void recv_server_reply1(int);
void recv_server_reply2(int);
int my_connect(char *servername, char *port);
void print_socket_address(int sd);

#define BUFFER_SIZE 2000

int main (int argc, char* const argv[]) {
	
	char buffer[BUFFER_SIZE];
	char mensagem[BUFFER_SIZE];
	char *line1, *line2;
	int nbytes;

	if(argc!=3) {
		printf("Usage: %s server_ip_address port_number\n",argv[0]);
		exit(1); 
	}

	//connect to server
	int socket_descriptor = my_connect(argv[1], argv[2]);
  
	//print local address
	print_socket_address(socket_descriptor);

	// Sending student number
    char *student_number = "1221709\n";
	strcpy(mensagem,student_number);
	printf("\nstudent_number, nbytes: %ld\n", strlen(student_number));
    
    // Reading message from stdin
    do {
		fflush(stdout);
		printf("\n");
		printf("Enter message (max 2000 bytes):\n");
        fgets(buffer, sizeof(buffer), stdin);

        if (strlen(buffer) <= 1 || strchr(buffer, '\n') == NULL) {
            continue;
        }

        break;
    } while (1); 
    printf("\nMensagem, nbytes: %ld\n", strlen(buffer));
    strcat(mensagem,buffer);
    printf("Mensagem completa, nbytes: %ld\n", strlen(mensagem));

    // Sending message to server
    nbytes = write(socket_descriptor, mensagem, strlen(mensagem));
    if (nbytes == -1) {
        perror("error write\n");
        close(socket_descriptor);
        exit(1);
    }

    // Receiving response from server
    char response[BUFFER_SIZE];
    
    nbytes = read(socket_descriptor, response, sizeof(response));
    if (nbytes == -1) {
        perror("read");
        close(socket_descriptor);
        exit(1);
    }
    response[nbytes] = '\0';

	line1 = strtok(response, "\n");
    if (line1 == NULL) {
        printf("Error parsing the Mensagem\n");
    }

    // Second strtok call using NULL to continue from the previous token
    line2 = strtok(NULL, "\n");
    if (line2 == NULL) {
        printf("Error parsing the Nome\n");
    }
    
	//Receive server reply. Check the difference between both versions of the routine.
	//recv_server_reply1(socket_descriptor);
	//recv_server_reply2(socket_descriptor); 
	
	printf("\nMensagem: %s\n""Nome: %s\n""Total nbytes: %d\n", line1, line2, nbytes);

	close(socket_descriptor);

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
    perror("ERROR socket");  
    exit(2);
  }
  
  //connect to server
  r = connect(s, addrs->ai_addr, addrs->ai_addrlen);
  if (r == -1) {
    perror("ERROR connect");  
    close(s);
    exit(3);
  }

  freeaddrinfo(addrs);  
  return s;
}

 //version 1 - prints server reply as it receives it (by parts)
void recv_server_reply1(int s) {
  char buf[4096];

  printf("Reply from server: \n");
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
void recv_server_reply2(int s) {
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
  printf("Reply from server: \n%s\n\n", buf);
}

void print_socket_address(int sd){
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

