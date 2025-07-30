#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// "%.2lf" na formatação do printf

#define veiculos_max 81
typedef struct {
	char matricula[6];
	char dono[veiculos_max];
	double valor;
} vehic_t;

void vehic_print(vehic_t *v){
	printf("\nMatricula:	%.6s \n", v->matricula);
	printf("Dono:		%s", v->dono);
	printf("Valor:		%.2lf \n\n", v->valor);
}

void vehic_read(vehic_t *v){
	char buf[100];
	
	printf("Inserir Matricula: ");
	fgets(buf, 8, stdin);
	buf[7] = 0; 
	memcpy(v->matricula, buf,6);

	printf("Inserir Dono: ");
	fgets(buf, veiculos_max+2, stdin);
	buf[veiculos_max-1] = 0; 
	strcpy(v->dono, buf);

	printf("Inserir Valor: ");
	fgets(buf,17, stdin);
	buf[16] = 0; 
	v->valor = atof(buf);
}

int main(){
	vehic_t v1;
	vehic_read(&v1);
	vehic_print(&v1);
	return 0;
}
