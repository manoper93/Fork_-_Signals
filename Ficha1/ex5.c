#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

void minha_divisao(int a, int b, int *ptr_quoiciente, int *ptr_resto){
	*ptr_quoiciente = a/b;
	*ptr_resto = a % b;
}

int main(int argc, char* argv[]){
	int a = atoi(argv[0]);
	int b = atoi(argv[1]);
	int quoiciente;
	int resto;

	minha_divisao(a, b, &quoiciente, &resto);
	printf("Resultado: %d resto: %d", quoiciente, resto);
	
	return(0);
}
