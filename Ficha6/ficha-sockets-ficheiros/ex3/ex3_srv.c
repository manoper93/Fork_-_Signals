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

#define BUFFER_SIZE 2000

int my_create_server_socket(char *port);
void print_address(const struct sockaddr * clt_addr, socklen_t addrlen);

int main(int argc, char *argv[]){
	
	char buffer[BUFFER_SIZE];
	char mensagem[BUFFER_SIZE];
    long int nbytes;
    char nomealuno[] = "Sergio da Silva Isidoro\n";
    char numeroaluno[BUFFER_SIZE];
    
  int new_socket_descriptor, socket_descriptor; 
  struct sockaddr clt_addr;
  socklen_t addrlen;
 
      
  if(argc!=2) {
    printf("Usage: %s port_number\n", argv[0]);
    exit(1); 
  }

  
  
  //SIGPIPE is sent to the process if a write is made to a closed connection.
  //By default, SIGPIPE terminates the process. This makes the process to ignore the signal.
  signal(SIGPIPE, SIG_IGN);
  socket_descriptor = my_create_server_socket(argv[1]);

  while(1) {
    
    printf("Waiting connection\n");  
  
    addrlen = sizeof(clt_addr);
    //accept a new connection to a client
    new_socket_descriptor = accept(socket_descriptor, &clt_addr, &addrlen); 
    if(new_socket_descriptor < 0) {
      perror("ERROR accept\n");
      sleep(1);
      continue;
    }
    print_address(&clt_addr, addrlen);
    
		pid_t pid = fork();
        if (pid < 0){
            perror("ERROR fork\n");
            exit(1);
        }
  
		if (pid == 0){ 						//filho
			printf("\n");
			
				struct timeval timeout;
                timeout.tv_sec = 5;
                timeout.tv_usec = 0;

                // Configurando o conjunto de descritores de arquivo para select
                fd_set read_fds;
                FD_ZERO(&read_fds);
                FD_SET(new_socket_descriptor, &read_fds);

                // Chamando select para verificar se há dados prontos para serem lidos
				while(select(new_socket_descriptor + 1, &read_fds, NULL, NULL, &timeout) <= 0){
					printf("Mensagem nao recebida em 5s\n");
					close(new_socket_descriptor);
					exit(1);
				}
				nbytes = read(new_socket_descriptor, buffer, sizeof(buffer));
				buffer[nbytes] = '\0';
				printf("buffer recebido, nbytes: %ld\n", strlen(buffer));
				
				char *line1 = strtok(buffer, "\n");
				if (line1 == NULL) {
					printf("Error parsing the Mensagem\n");
					close(new_socket_descriptor);
					exit(1);
				}

				// Second strtok call using NULL to continue from the previous token
				char *line2 = strtok(NULL, "\n");
				if (line2 == NULL) {
					printf("Error parsing the Nome\n");
					close(new_socket_descriptor);
					exit(1);
				}
				line2[strlen(line2)] = '\n';
				
				strcpy(numeroaluno,line1);
				printf("Numero aluno recebido, nbytes: %ld\n", strlen(line1));
				if(atoi(line1) == 1221709) {
					printf("Nome de aluno= %s", nomealuno);
					printf("Nome de aluno, nbytes: %ld\n\n", strlen(nomealuno));
				} else {
					strcpy(nomealuno,"Desconhecido\n");
					printf("Nome de aluno= %s", nomealuno);
					printf("Nome de aluno, nbytes: %ld\n\n", strlen(nomealuno));
				}
				
				strcpy(mensagem,line2);
				printf("Mensagem recebida= %s", mensagem);
				printf("Mensagem recebida, nbytes: %ld\n", strlen(mensagem));
				
				for (int i = 0; i < strlen(mensagem); i++) {
					mensagem[i] = toupper(mensagem[i]);
				}
				
				strcat(mensagem,nomealuno);

				if (write(new_socket_descriptor, &mensagem, strlen(mensagem)) == -1) {
					perror("ERROR write mensagem\n");
					close(new_socket_descriptor);
					exit(1);
				} else {
					printf("Mensagem enviada, nbytes: %ld\n", strlen(mensagem));
					close(new_socket_descriptor);
					exit(0);
				}
				

			close(new_socket_descriptor);
			exit(0);
			
            
        } else { 							//pai
			close(new_socket_descriptor); 
		}
		
	}
	close(socket_descriptor);
  
  return 0;
}

void print_address(const struct sockaddr * clt_addr, socklen_t addrlen) {
	
  char hostname[256];  
  char port[6];

  int n = getnameinfo(clt_addr, addrlen, hostname, sizeof(hostname), 
    port, sizeof(port), NI_NUMERICHOST | NI_NUMERICSERV); 
  if(n != 0)
    printf("%s\n", gai_strerror(n));
  else
    printf("Connection from %s:%s\n", hostname, port);
}

int my_create_server_socket(char *port){
	
  int s, r;
  struct addrinfo hints, *a;

  memset (&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_flags = AI_PASSIVE;
  r = getaddrinfo(NULL, port, &hints, &a);
  if (r != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(r));
    exit(1);
  } 


  s = socket(PF_INET, SOCK_STREAM, 0);
  if(s == -1){
    perror("ERROR socket");
    exit(1);
  }


  //avoid bind errors if the port is still being used by previous connections
  int so_reuseaddr = 1;
  setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &so_reuseaddr, sizeof(so_reuseaddr));

  
  r = bind(s, a->ai_addr, a->ai_addrlen);
  if(r == -1) {
    perror("bind");
    exit(1);
  }    
  
  r = listen(s, 5); 
  if(r == -1) {
    perror("listen");
    exit(1);
  }   
  
  freeaddrinfo(a);
  return s;
}

/*
			//------------------------------------------ enviar
			
			nbytes = read(new_socket_descriptor, buffer, sizeof(buffer));
			
			for (int i = 0; i < nbytes; i++) {
				char text_char = buffer[i];
				
				text_char = toupper(text_char);
				
				int write_result = write(new_socket_descriptor, &text_char, 1);
				if (write_result == -1) {
					perror("Error writing to socket");
					return 1;
				}
				
				if (i == nbytes) {
					printf("Enviado!!\n");
				}
			}

			close(new_socket_descriptor); 
			//------------------------------------------ fim de enviar
			* 
			* 
			* 
			* 				// Check for newline character
				if (text_char == '\n') {
					printf("nbytes= %d\n", nbytes);
					printf("Found newline character at position %d\n", i);
				}
			*/




