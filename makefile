CC  = gcc -Wall -Wextra
BIN = Sample
INC = includes/
CFLAGS =`sdl-config --libs --cflags` -lESDL
CSDL = `sdl-config --cflags`

$(BIN): main.o
	$(CC) main.o -o $(BIN) -lm $(CFLAGS)

main.o: main.c
	$(CC) -c main.c $(CSDL)

clean:
	@rm -f *.o
	@rm $(BIN)