CC      = gcc
INCLUDE = ../include
CCFLAGS = -O2 -I$(INCLUDE) -Wall -Wextra -Werror -pedantic-errors
OBJECT  = crc.o pnggen.o main.o


all: $(OBJECT)
	$(CC) $(CCFLAGS) -o ../rig $(OBJECT) -lm

%.o: %.c
	$(CC) $(CCFLAGS) -c $<

clean:
	rm -rf *.o
