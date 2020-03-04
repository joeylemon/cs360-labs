CC = gcc 
CFLAGS = -m32 -g

mymalloc: mymalloc.c mymalloc.h
	$(CC) $(CFLAGS) -o mymalloc mymalloc.c

#make clean will rid your directory of the executable,
#object files, and any core dumps you've caused
clean:
	rm -rf core mymalloc *.o
