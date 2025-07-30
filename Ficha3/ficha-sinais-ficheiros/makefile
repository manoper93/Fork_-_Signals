CFLAGS = -Wall

TARGETS = ex1 ex2 ex3 ex4 sigchld test_sigfpe 

all: $(TARGETS)

deploy: $(TARGETS) ficha-sinais-ficheiros.zip

ficha-sinais-ficheiros.zip: ex1.c ex2.c ex3.c ex4.c sigchld.c test_sigfpe.c makefile
	-rm $@
	zip $@ $^

clean:
	-rm *.o $(TARGETS) *~
	
