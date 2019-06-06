cc = gcc
CFLAGS = -g -Wall# -ansi -pedantic #-g debugowanie , -ansi standard, -pedantic pedantyczne sprawdzanie
OBJECTS =
TARGET = huffman
INCLUDES = huffman.h

default: $(TARGET)

.c.o:
	$(cc) $(CLAGS) -c $< -o $*.o

encode: huffman.c $(INCLUDES)
	$(cc) $(CFLAGS) huffman.c -lm -o huffman
	
