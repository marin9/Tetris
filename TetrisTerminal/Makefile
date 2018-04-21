CC=gcc
FLAGS=-Wall -Wextra -c
OBJ=main.c graphic.o tetris.o
TARGET=tetris


all: $(TARGET)

run:
	make
	./$(TARGET)


$(TARGET) : $(OBJ)
	@echo "Target:" $(TARGET)
	@gcc $(OBJ) -o $(TARGET)

%.o : %.c %.h
	@echo "Compile:" $<
	@$(CC) $(FLAGS) $< -o $@


clean:
	@echo "Clean"
	@rm -f *.o $(TARGET)
