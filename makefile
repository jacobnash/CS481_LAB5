EXECUTABLES = myls

CC = gcc -std=gnu99 
CFLAGS = -O2 

all: $(EXECUTABLES)

.SUFFIXES: .c .o
.c.o:
	$(CC) $(CFLAGS)  -o $*.c -lm

clean:
	rm -f *.o $(EXECUTABLES) *~
test: all
	#
	./myls -lf text.txt

