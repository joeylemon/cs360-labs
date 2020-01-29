CLASSDIR = /home/plank/cs360
INCLUDE = -I$(CLASSDIR)/include
LIBS = $(CLASSDIR)/objs/libfdr.a
CC = gcc

EX = famtree
all: $(EX) 
clean: 
	rm -f $(EX) *.o

famtree: famtree.o $(LIBS)
	$(CC) $(INCLUDE) -o famtree famtree.o $(LIBS)


.SUFFIXES: .c .o

.c.o: 
	$(CC) $(INCLUDE) -g  -c  $*.c
