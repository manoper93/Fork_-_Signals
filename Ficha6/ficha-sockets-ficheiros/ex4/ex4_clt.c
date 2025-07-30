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
void clean_stdin();

typedef struct {
	char student_id[7];
	char text[2000]; //should be ‘\0’ terminated
} msg1_t;

typedef struct {
	char text[2000]; //should be ‘\0’ terminated
	char student_name[100]; //should be ‘\0’ terminated
} msg2_t;

#define BUFFER_SIZE 2000

int main (int argc, char* const argv[]) {
	
	msg1_t msg1;
	msg2_t msg2;

	if(argc!=3) {
		printf("Usage: %s server_ip_address port_number\n",argv[0]);
		exit(1); 
	}

	//connect to server
	int socket_descriptor = my_connect(argv[1], argv[2]);
  
	//print local address
	print_socket_address(socket_descriptor);
	
	// Sending student number
    strcpy(msg1.student_id, "1221709");
	
	// Reading message from stdin
    do {
		printf("\n");
		printf("Enter message (max 2000 bytes):\n");
        fgets(msg1.text, sizeof(msg1.text), stdin);

        if (strlen(msg1.text) <= 1 || strchr(msg1.text, '\n') == NULL) {
            continue;
        }
		
        break;
    } while (1);

	printf("\n");
	
    printf("MSG1.student_id= %.7s\n", msg1.student_id);
	printf("MSG1.text= %s", msg1.text);
	printf("MSG1.text, nbytes: %ld\n", strlen(msg1.text));
	
	printf("\n");

    // Sending message to server
    int nbytes = write(socket_descriptor, &msg1, sizeof(msg1));
    if (nbytes == -1) {
        perror("error write\n");
        close(socket_descriptor);
        exit(1);
    }

    // Receiving response from server
	FILE * fp = fdopen(socket_descriptor, "r+");
	nbytes = fread(&msg2, 1, sizeof(msg2_t), fp);
	
	
	printf("MSG2.text= %s", msg2.text);
	printf("MSG2.text, nbytes: %ld\n", strlen(msg2.text));
	printf("MSG2.student_name= %s\n", msg2.student_name);
	printf("MSG2.student_name, nbytes: %ld\n", strlen(msg2.student_name));
	
	printf("\n");
    
    printf("Mensagem: %s""Nome: %s\n""Total nbytes: %ld\n", msg2.text, msg2.student_name, strlen(msg2.text)+strlen(msg2.student_name));
	
	//Receive server reply. Check the difference between both versions of the routine.
	//recv_server_reply1(socket_descriptor);
	//recv_server_reply2(socket_descriptor); 

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

