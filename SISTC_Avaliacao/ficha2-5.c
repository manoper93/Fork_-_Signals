#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]) {
	printf("1221709 - %s\n", __FILE__);
	
  char *ptr = malloc(8); // Alocação de 8 bytes na memória de dados
  strcpy(ptr, "pai"); // Armazenamento da string "pai"

  pid_t r = fork(); // Criação de um processo filho

  if (r == 0) { // Processo filho
    strcpy(ptr, "filho"); // Escrita da string "filho" no bloco
    printf("Conteúdo do bloco no processo: %s\n", ptr); // Impressão do conteúdo do bloco
    exit(0); // Término do processo filho
  }
  
  waitpid(r, NULL, 0); // Aguarda a conclusão do processo filho
  printf("Conteúdo do bloco no processo: %s\n", ptr); // Impressão do conteúdo do bloco

  return 0;
}


/*

1221709 - ficha2-5.c
* Como a variavel ptr não está indicada como apontador de endereço o filho nao tem acesso ao mesmo endereço que o do pai.
Conteúdo do bloco no processo: filho
Conteúdo do bloco no processo: pai

*/
