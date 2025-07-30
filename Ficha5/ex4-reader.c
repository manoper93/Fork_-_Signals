#include "mycommon.h"
#include <semaphore.h>

sem_t *psem; //

int main(int argc, char** argv) {
  
  int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
  if (shm_fd == -1) handle_error("shm_open");

  int r = ftruncate(shm_fd, SHM_SIZE);
  if (r == -1) handle_error("ftruncate");

  char* shm_ptr = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  if (shm_ptr == MAP_FAILED) handle_error("mmap");

	psem = sem_open("/sem1", O_CREAT, 0666, 0); //
  strcpy(shm_ptr, "");
  
  while (1) {
	sem_wait(psem); //
    printf("Dados: %s\n", shm_ptr);
    sleep(2);
  }
  
	sem_close(psem); //
	sem_unlink("/sem1"); //
   
  return 0;
}

// gcc -Wall ex4-reader.c -o ex4r
