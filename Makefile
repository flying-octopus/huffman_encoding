cc = gcc
CFLAGS = -g -Wall# -ansi -pedantic #-g debugowanie , -ansi standard, -pedantic pedantyczne sprawdzanie
OBJECTS =
TARGET = encode
INCLUDES = encode.h

default: $(TARGET)

.c.o:
	$(cc) $(CLAGS) -c $< -o $*.o

encode: encode.c $(INCLUDES)
	$(cc) $(CFLAGS) encode.c -lm -o encode
	
clean:
	rm -rf *.o

mrproper:
	
