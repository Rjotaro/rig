CC = gcc
OBJECT = crc.o pnggen.o main.o

all: $(OBJECT)
	$(CC) -o rig $(OBJECT) -lm
	./rig
	xxd image.png > dumb
	
pnggen.o: pnggen.c pnggen.h
	$(CC) -c pnggen.c
	
crc.o: crc.c crc.h
	$(CC) -c crc.c
	
main.o: main.c
	$(CC) -c main.c
	
hex: image.png
	xxd image.png > dump
	
clean:
	rm *.o image.png dumb
