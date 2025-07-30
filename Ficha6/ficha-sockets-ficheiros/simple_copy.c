#include <stdio.h>
#include <stdlib.h>

int main (int argc, char* const argv[]) {
  char buffer[4096];
  
  if(argc!=3)
  {
    printf("usage: %s source destination\n", argv[0]);
    return 0;
  }
    
  FILE *fp1 = fopen(argv[1], "r");
  if(fp1 == NULL) {
    perror("fopen source");
    exit(1);
  }

  FILE *fp2 = fopen(argv[2], "w");
  if(fp1 == NULL) {
    perror("fopen destination");
    exit(1);
  }
  
  while(1) {
    //Reads at most sizeof(buffer) bytes from fp1 into the buffer
    int n = fread(buffer, 1, sizeof(buffer), fp1);
    if(n <= 0)
      break;
      
    //Writes n bytes contained in buffer into fp2 
    fwrite(buffer, 1, n, fp2);
  }
  
  return 0;
}
