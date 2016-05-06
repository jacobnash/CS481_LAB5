EXECUTABLES = myls

CC = gcc -Wall -Werror -std=gnu99 
LIBS = libfdr.a
CFLAGS = -O2

all: $(EXECUTABLES)

.SUFFIXES: .c .o
.c.o:
	$(CC) $(CFLAGS) -c $*.c

clean:
	rm -f *.o $(EXECUTABLES) *~
test: all
	#
	./myls -lf text.txt

