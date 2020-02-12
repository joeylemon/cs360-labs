#CS 360 Lab 1 Makefile

CC = gcc 
INCLUDES = -I/home/jplank/cs360/include
CFLAGS = -g $(INCLUDES)
LIBDIR = /home/jplank/cs360/objs
LIBS = $(LIBDIR)/libfdr.a 
EXECUTABLES = fakemake

all: $(EXECUTABLES)

.SUFFIXES: .c .o
.c.o:
	$(CC) $(CFLAGS) -c $*.c

$(EXECUTABLES): $(EXECUTABLES).o
	$(CC) $(CFLAGS) -o $(EXECUTABLES) $(EXECUTABLES).o $(LIBS)

#make clean will rid your directory of the executable,
#object files, and any core dumps you've caused
clean:
	rm -rf core $(EXECUTABLES) *.o