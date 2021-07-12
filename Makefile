CC = gcc
CCFLAGS = -O2 -Wall -Wextra -Werror -pedantic-errors
OBJECT = crc.o pnggen.o main.o
	
all: $(OBJECT)
	$(CC) $(CCFLAGS) -o rig $(OBJECT) -lm
	./rig
	xxd image.png > dumb

debug: CCFLAGS += -DDEBUG
	
debug: all
	
pnggen.o: pnggen.c pnggen.h
	$(CC) $(CCFLAGS) -c pnggen.c
	
crc.o: crc.c crc.h
	$(CC) $(CCFLAGS) -c crc.c
	
main.o: main.c
	$(CC) $(CCFLAGS) -c main.c
	
hex: image.png
	xxd image.png > dump
	
clean:
	rm -rf *.o image.png dumb rig
