#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

void wait_something();
void do_something();
void myHandler(int signum);

int main() {
  printf("1221709 - %s\n", __FILE__);
  
  sigset_t mask;
  sigemptyset(&mask);
  sigaddset(&mask, SIGINT);
  sigaddset(&mask, SIGQUIT);
  sigprocmask(SIG_BLOCK, &mask, NULL);

  struct sigaction act;
  act.sa_handler = myHandler;
  sigemptyset(&act.sa_mask);
  act.sa_flags = SA_RESETHAND;
  sigaction(SIGCHLD, &act, NULL);
  
  if(sigaction(SIGCHLD, &act, NULL) == -1){
	perror("sigaction");
	exit(1);
  }
 
  while(1) {
   wait_something();
   
   pid_t r = fork();
   if(r == 0) {
	 sigemptyset(&mask);
     sigaddset(&mask, SIGTSTP);
     sigprocmask(SIG_BLOCK, &mask, NULL);
     
	 signal(SIGTERM, SIG_IGN);
	 
     do_something();
     
     exit(0);
   }
  }
  
  return(0);
}

void wait_something() {
  sleep(1);
}

void do_something() {
  sleep(1);
}

void myHandler(int signum) {
  int wstatus;
  
  wait(&wstatus);
  
  if( WIFSIGNALED(wstatus) )
    printf("Child terminated by signal\n");
}
