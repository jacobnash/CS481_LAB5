EXECUTABLES = myls

CC = gcc
LIBS = libfdr.a
CFLAGS = -O2

all: $(EXECUTABLES)

.SUFFIXES: .c .o
.c.o:
	$(CC) $(CFLAGS) -Wall -Werror -c $*.c

clean:
	rm -f *.o $(EXECUTABLES) *~
test: all
	#
	./myls -dhl
##
	ls -s
	#
