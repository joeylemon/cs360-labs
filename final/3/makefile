CC = gcc 
CFLAGS = -g -pthread $(INCLUDES)
EXECUTABLES = three

all: $(EXECUTABLES)

.SUFFIXES: .c .o
.c.o:
	$(CC) $(CFLAGS) -c $*.c

three: three.o
	$(CC) $(CFLAGS) -o three three.o

#make clean will rid your directory of the executable,
#object files, and any core dumps you've caused
clean:
	rm -rf core $(EXECUTABLES) *.o tmp*.txt output-*.txt