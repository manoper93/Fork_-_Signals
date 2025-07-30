#ifndef INCLUDE_MYCOMMON_H
#define INCLUDE_MYCOMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#define SHM_NAME "/f5-ex4"
#define SHM_SIZE 1024

#define handle_error(s) { perror(s); exit(EXIT_FAILURE);}

#endif
