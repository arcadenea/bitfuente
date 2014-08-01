CC = g++
DEBUG = -g
CFLAGS = -Wall `sdl-config --cflags` -I./ -c $(DEBUG) -fPIC
LFLAGS = -Wall $(DEBUG) 
LIBS = `sdl-config --libs` -lm  -lSDL_ttf -lpng 
prefix=/usr/lib

OBJECTS = bitfuente.o savepng.o

all: $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) -o bitfuente $(LIBS)

  
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o bitfuente
