#include <pthread.h>
#include <sys/types.h> 
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */

void myprint(const char *str);
void *myfunc(void *);
sem_t *psem;

int main()
{
  char *buf = (char *) malloc(256);
  sem_unlink("/sem1");
  psem = sem_open ("/sem1", O_CREAT | O_RDWR, 0600, 1);

  int r = fork();
  if(r==0){
    
    sprintf(buf,"%d: Ola -----------------------------------------\n",getpid());  
    myfunc(buf);
    
  } else {
  
    sprintf(buf,"%d: Ole +++++++++++++++++++++++++++++++++++++++++\n",getpid());
    myfunc(buf);
    
  }
  
  sem_close(psem);
  return(0);
}

void *myfunc(void *arg){
  while(1) {
    sleep(1);   
    sem_wait(psem);
    myprint((char *)arg);
    sem_post(psem);
    
  }
}


