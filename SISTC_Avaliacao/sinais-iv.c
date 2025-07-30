#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

void fun1(int *d) {
  ++d[0];
  printf("Novo valor gerado em fun1: %d\n", d[0]);
  sleep(2);
}

void fun2(int *d) {
  sleep(1); 
  printf("Valor processado por fun2: %d\n", d[0]);
}

void sigchld_handler(int sig) {
  while (waitpid(-1, NULL, WNOHANG) > 0) {}
}

int main(){
  printf("1221709 - %s\n", __FILE__);
	
  int dados = 0;
  
  struct sigaction act;
  act.sa_handler = sigchld_handler;
  sigemptyset(&act.sa_mask);
  act.sa_flags = SA_NOCLDWAIT;
  sigaction(SIGCHLD, &act, NULL);
  
  if(sigaction(SIGCHLD, &act, NULL) == -1){
	perror("sigaction");
	exit(1);
  }

  while(1) {
    fun1(&dados);
    pid_t r = fork();
    if(r == -1) {
      perror("fork");
      exit(1);
    }
    if(r==0) {
      fun2(&dados);
      exit(0);
    }
  }
}


