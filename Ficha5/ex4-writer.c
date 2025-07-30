#include "mycommon.h"
#include <semaphore.h>

sem_t *psem; //

int main(int argc, char** argv){
  char buffer[SHM_SIZE];
  
  int shm_fd = shm_open(SHM_NAME, O_RDWR, 0);
  if (shm_fd == -1) handle_error("shm_open");

  char* shm_ptr = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  if (shm_ptr == MAP_FAILED) handle_error("mmap");
  
  psem = sem_open("/sem1", O_CREAT, 0666, 0); //

  printf(": ");
  fgets(buffer, SHM_SIZE, stdin);
  strcpy(shm_ptr, buffer);
  
  sem_post(psem); //
  sem_close(psem); //
  sem_unlink("/sem1"); //
  
  return 0;
}

// gcc -Wall ex4-writer.c -o ex4w
