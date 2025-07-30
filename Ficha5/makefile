CFLAGS = -Wall
LDFLAGS = -pthread


ex2: ex2.o myprint.o 

all: ex2 ficha-sinc-ficheiros.zip

ficha-sinc-ficheiros.zip: myprint.c ex2.c makefile
	-rm $@
	zip $@ $^

clean:
	-rm *.o ex2 *.zip
