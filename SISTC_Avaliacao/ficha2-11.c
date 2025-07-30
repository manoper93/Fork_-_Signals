#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

void fun1(int *d) {
	++d[0];
	printf("Novo valor gerado em fun1: %d\n", d[0]);
	sleep(2);
}

void fun2(int *d) {
	sleep(1);
	printf("Valor processado por fun2: %d\n", d[0]);
}

int main(){
	printf("1221709 - %s\n", __FILE__);
	
	pid_t r;
	int dados = 0;
	
	
	
	while(1) {
		fun1(&dados);
			
		r = fork(); 
		if(r == 0){
			fun2(&dados);
			exit(0);
		}
	}
}

/*

1221709 - ficha2-11.c
Novo valor gerado em fun1: 1
Novo valor gerado em fun1: 2
Valor processado por fun2: 1
Novo valor gerado em fun1: 3
Valor processado por fun2: 2
...

*/
