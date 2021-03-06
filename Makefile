CC = gcc
CFLAGS = -Wall -std=c11 -pedantic -W
BIN = projekt
RM = rm -f -v
OBJECTS = main.o scaner.o garbage.o parser.o semantic.o

all: $(OBJECTS) error.h 
	$(CC) $(CFLAGS) $(OBJECTS) -o $(BIN)

scaner.o: scaner.c scaner.h
main.o: main.c
parser.o: parser.c parser.h
garbage.o: garbage.c garbage.h
semantic.o: semantic.c semantic.h


clean:
	$(RM) *.o $(BIN)
