#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/**********************************************************************
 * This file contains hints on how to convert it to a client/server 
 * architecture
 *********************************************************************/
 
int main (int argc, char* const argv[]) {
  char buffer[4096];

  
  if(argc!=2) {
    printf("usage: %s source\n", argv[0]);
    return 1;
  }
  
  
  //Hint1: client side (do this before connection to server)
  int fds = open(argv[1], O_RDONLY);
  if(fds == -1) {
    perror("open source");
    return 1;
  }

  //Hint2: server side (this should be done after connection establishment)
  char filename[11];
  strcpy(filename, "fileXXXXXX");
  int fdd = mkstemp(filename);
  if(fdd == -1){
    perror("open destination");
    return 1;
  }
 
 

  while(1) {
    
    /*****************************************************************
     * Client side
     ****************************************************************/
    
    //Reads at most sizeof(buffer) bytes from fds into the buffer
    int n = read(fds, buffer, sizeof(buffer));
    if(n <=  0) //means the end of file was reached
      break;

    //Hint3: client must write buffer contents to socket
    //To do...
      
      
      
    /*****************************************************************
     * Server side
     ****************************************************************/
      
    //Hint4: server must read data from socket into the buffer
    //To do...
    
      
    //Writes n bytes contained in the buffer into fdd
    write(fdd, buffer, n);
  }
  
  close(fds);
  close(fdd);
  
  printf("File %s created\n", filename);
  
  return 0;
}
