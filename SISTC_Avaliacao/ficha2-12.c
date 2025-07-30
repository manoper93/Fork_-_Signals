#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h> 
#include <float.h>
#include <sys/wait.h>

//NELEM must be a multiple of 2
#define NELEM 20000000
#define NITER 51

void rectangular2polar(double *, double *, int); 
double mytime();

int main() {
	printf("1221709 - %s\n", __FILE__);
	pid_t r[31];
  double t0;  

  double *dados_in = (double *) malloc(sizeof(double)*NELEM); 
  double *dados_out = (double *) malloc(sizeof(double)*NELEM);
  if(dados_in == NULL || dados_out == NULL) { 
    perror("malloc"); 
    exit(1); 
  } else {
    //assign random initial values
    double maxv = DBL_MAX;
    srand48(time(NULL));
    for(int i = 0; i < NELEM; ++i)
      dados_in[i] = 2*(drand48()-0.5)*maxv;
  }

  //get current time, for benchmarking 
  t0 = mytime();   

  //This cycle is used only for benchmarking purposes
	for(int i=0; i < NITER; ++i)   {
	   
		for(int j=0; j < 30;j++){
			r[j] = fork();
			if(r[j] == 0){
				rectangular2polar(dados_out+j*NELEM/30, dados_in+j*NELEM/30, NELEM/30);
				exit(0);
			}
		}
		
		for(int k=0; k < 30;k++){
			waitpid(r[k], NULL, 0);
		}
	}
  
  printf("Computation took %.1f s\n", mytime() - t0);
 
  return 0; 
} 

double mytime() {
  struct timeval tp;
  struct timezone tzp;

  gettimeofday(&tp,&tzp);
  return ( (double) tp.tv_sec + (double) tp.tv_usec * 1.e-6 );
}

/*
gcc -o ficha2-12 ficha2-12.c ficha2-12math.c -lm -O3

b)
1221709 - ficha2-12.c
Computation took 10.0 s

c)
1221709 - ficha2-12.c
Computation took 3.4 s

*/
