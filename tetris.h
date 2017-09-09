#ifndef TETRIS_H
#define TETRIS_H

char collision(char _x, char _y, char _current[4][4]);
char fallingPossible();
char createNewPieces();
void fixToBoard();
void checkFullRows();

void reset(char level, char show_next);
void step();
void rotate();
void down();
void left();
void right();
void setPause(char pause);
char isPaused();
char isGameOver();
char* getBoard();
int getScore();
char getLevel();
char getCurrentX();
char getCurrentY();
char* getCurrentType();
char* getNext();

#endif

