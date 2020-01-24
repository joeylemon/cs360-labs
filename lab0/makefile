all:

CC = gcc 
CFLAGS = -g -Wall -lm

BIN = chain_heal

all: $(BIN)

.cpp: 
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(BIN)
