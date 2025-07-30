#include <stdio.h> 
#include <stdlib.h> 
#include <math.h> 
#include <time.h> 
#include <sys/time.h> 
#include <sys/resource.h> 
#include <float.h>
#include <pthread.h>
#include <sys/types.h> 
#include <unistd.h>


#define NUM_ELEM  200
#define NUM_ITER  20000000
#define N_THREADS 50

void operacao_muito_demorada(double *, int); 
void print_statistics(time_t t0);
void print_data(double *dados);
void* aux(void* args);

typedef struct {
double *ptr;
int n;
} targs_t;

void* aux(void* args){
targs_t* p = (targs_t*) args;
operacao_muito_demorada(p->ptr, p->n);
return NULL;
}

int main() { 
  pthread_t tids[N_THREADS];
  targs_t args[N_THREADS];
  
  //Allocation of a vector to hold the data to be processed
  double *dados = malloc(sizeof(double)*NUM_ELEM); 
  if(dados==NULL) 
  { 
    perror("malloc"); 
    exit(1); 
  }
  else
  {
    for(int i = 0; i < NUM_ELEM; ++i)
      dados[i] = i*1.0;
      
    printf("%.1lf KiB allocated\n", sizeof(double)*NUM_ELEM/1024.0); 
  }

  //get current time, for benchmarking
  time_t t0;   
  t0 = time(NULL); 

	args[0].ptr = dados;
	args[0].n = NUM_ELEM/N_THREADS;

  for(int i=0; i < N_THREADS; i++) {
	args[i].ptr = dados+i*NUM_ELEM/N_THREADS;
	args[i].n = i*NUM_ELEM/N_THREADS;
	pthread_create(&tids[i], NULL, aux, &args[i]);
  }
  
  for(int i=0; i < N_THREADS; i++) {
	pthread_join(tids[i], NULL);
  }
  
  print_statistics(t0);

  //this should print the sequence 0, 2, 4, ... , 2*(NUM_ELEM-1)
  print_data(dados);
  
  return 0; 
} 

 void print_statistics(time_t t0)
{
  struct rusage usage;    
  
  if(getrusage(RUSAGE_SELF , &usage)==-1) 
    perror("getrusage"); 
         
  printf("%ld seconds elapsed\n", time(NULL)-t0); 
  printf("user time: \t %3ld.%06ld s\n", 
    usage.ru_utime.tv_sec,usage.ru_utime.tv_usec);     
  printf("system time: \t %3ld.%06ld s\n", 
    usage.ru_stime.tv_sec,usage.ru_stime.tv_usec); 
  printf("total: %.3f seconds\n", 
    usage.ru_utime.tv_sec + usage.ru_utime.tv_usec/1.0e6 +
    usage.ru_stime.tv_sec + usage.ru_stime.tv_usec/1.0e6); 
}

void print_data(double *dados)
{
  int i;
  printf("\n");
  for(i = 0; i < NUM_ELEM-1; ++i)
    printf("%.0lf, ", dados[i]);
  printf("%.0lf\n\n", dados[i]);
}


//Prenche o vector com dados desde o endereço base (vector) até vector+tamanho_vector-1
void operacao_muito_demorada(double *vector, int tamanho_vector){ 
  int i, j;   
  double *v; 
  printf("processing %d elements\n",tamanho_vector); 
  double val = vector[0];
  
  
  //Apenas para consumir tempo
  for(j=0;j<NUM_ITER;++j) { 
    v=vector; 
    for(i=0;i<tamanho_vector;++i) {  
      *v=drand48(); 
      v++; 
    } 
    v=vector;    
    for(i=0;i<tamanho_vector;++i) { 
      *v=sin(pow(*v,3)); 
      v++; 
    } 
  } 

  //Escrita no vector
  for(i = 0; i < tamanho_vector; ++i)
  {
    vector[i] = (val + i)*2.0;
  }
}

/*1)
 * 
1.6 KiB allocated
processing 200 elements
6 seconds elapsed
user time: 	   6.312866 s
system time: 	   0.000999 s
total: 6.314 seconds


 * 1.1)
 * 
1.6 KiB allocated
processing 200 elements
124 seconds elapsed
user time: 	 124.336377 s
system time: 	   0.006926 s
total: 124.343 seconds


 * 1.2)
 * 
 1.6 KiB allocated
processing 100 elements
processing 100 elements
67 seconds elapsed
user time: 	 132.960164 s
system time: 	   0.005832 s
total: 132.966 seconds

 
 *
 */
 

