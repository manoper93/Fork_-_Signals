CFLAGS = -Wall
LDFLAGS = -lrt -pthread -lm

default: ex1 par ficha-threads-ficheiros.zip

ex1: ex1.o myprint.o 

par: par.o
	gcc par.o -o par $(LDFLAGS)

ficha-threads-ficheiros.zip: ex1.c myprint.c par.c makefile
	-rm $@
	zip $@ $^

clean:
	-rm *.o ex1 par
