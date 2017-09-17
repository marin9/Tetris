CC=gcc
FLAGS=-Wall -Wextra -c
OBJ=main.c graphic.o audio.o tetris.o

all: tetris

test:
	make
	./tetris


tetris : $(OBJ)
	gcc $(OBJ) -o tetris -lSDL2

%.o : %.c
	$(CC) $(FLAGS) $< -o $@


clean:
	rm *.o tetris
