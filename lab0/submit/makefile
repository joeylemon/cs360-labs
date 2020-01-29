all:

CC = /usr/bin/gcc 
CFLAGS = -Wall -g -lm 

BIN = chain_heal

all: $(BIN)

.cpp: 
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(BIN)
