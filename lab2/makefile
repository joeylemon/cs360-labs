all:

CC = gcc 
CFLAGS = -g -Wall -lm

BIN = data_uncompress

all: $(BIN)

.cpp: 
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(BIN)
