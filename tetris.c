#ifndef TETRIS_H
#define TETRIS_H

#include <stdlib.h>
#include "tetris.h"

#define COLUMNS		10
#define ROWS		20


static char const O[1][4][4]={ {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}  }};

static char const I[2][4][4]={ {{0, 0, 0, 0}, {2, 2, 2, 2}, {0, 0, 0, 0}, {0, 0, 0, 0}},
              			   {{0, 0, 2, 0}, {0, 0, 2, 0}, {0, 0, 2, 0}, {0, 0, 2, 0}} };

static char const S[2][4][4]={ {{0, 0, 0, 0}, {0, 0, 3, 3}, {0, 3, 3, 0}, {0, 0, 0, 0}},
              			   {{0, 0, 3, 0}, {0, 0, 3, 3}, {0, 0, 0, 3}, {0, 0, 0, 0}} };

static char const Z[2][4][4]={ {{0, 0, 0, 0}, {0, 4, 4, 0}, {0, 0, 4, 4}, {0, 0, 0, 0}},
               			   {{0, 0, 0, 4}, {0, 0, 4, 4}, {0, 0, 4, 0}, {0, 0, 0, 0}} };

static char const L[4][4][4]={ {{0, 0, 0, 0}, {0, 5, 5, 5}, {0, 5, 0, 0}, {0, 0, 0, 0}  },
               			   {{0, 0, 5, 0}, {0, 0, 5, 0}, {0, 0, 5, 5}, {0, 0, 0, 0}  },
               			   {{0, 0, 0, 5}, {0, 5, 5, 5}, {0, 0, 0, 0}, {0, 0, 0, 0}  },
               			   {{0, 5, 5, 0}, {0, 0, 5, 0}, {0, 0, 5, 0}, {0, 0, 0, 0}  }};

static char const J[4][4][4]={ {{0, 0, 0, 0}, {0, 6, 6, 6}, {0, 0, 0, 6}, {0, 0, 0, 0}  },
               			   {{0, 0, 6, 6}, {0, 0, 6, 0}, {0, 0, 6, 0}, {0, 0, 0, 0}  },
               			   {{0, 6, 0, 0}, {0, 6, 6, 6}, {0, 0, 0, 0}, {0, 0, 0, 0}  },
              			   {{0, 0, 6, 0}, {0, 0, 6, 0}, {0, 6, 6, 0}, {0, 0, 0, 0}  }};

static char const T[4][4][4]={ {{0, 0, 0, 0}, {0, 7, 7, 7}, {0, 0, 7, 0}, {0, 0, 0, 0}  },
               			   {{0, 0, 7, 0}, {0, 0, 7, 7}, {0, 0, 7, 0}, {0, 0, 0, 0}  },
              			   {{0, 0, 7, 0}, {0, 7, 7, 7}, {0, 0, 0, 0}, {0, 0, 0, 0}  },
               			   {{0, 0, 7, 0}, {0, 7, 7, 0}, {0, 0, 7, 0}, {0, 0, 0, 0}  }};


static char board[ROWS][COLUMNS];
static char current[4][4];
static char orientation;
static char currentX;
static char currentY;

static char gameOver;
static char pause;
static char showNext;
static char next;
static char drop;
static int score;
static int lines;
static char level;
static char _level;
static int count;



char collision(char _x, char _y, char _current[4][4]){
	for(short y=0;y<4;++y){
		for(short x=0;x<4;++x){
			short tx=_x+x;
			short ty=_y+y;

			if(_current[y][x]==0) continue;
			if(tx<0 || ty<0 || tx>=COLUMNS || ty>=ROWS) return 1;
			if(board[ty][tx]!=0)  return 1;
		}
	}
	return 0;
}


char fallingPossible(){
	return !collision(currentX, currentY+1, current);
}


char createNewPieces(){
	for(short y=0;y<4;++y){
		for(short x=0;x<4;++x){
			current[y][x]=0;
		}
	}

	char pieces=next;
	next=rand()%7+1;
	orientation=0;
	drop=-1;
	currentY=-1;
	currentX=3;

	switch(pieces){
		case 1: //O
			current[1][1]=1;
			current[1][2]=1;
			current[2][1]=1;
			current[2][2]=1;
			break;

		case 2: //I
			current[1][0]=2;
			current[1][1]=2;
			current[1][2]=2;
			current[1][3]=2;
			break;

		case 3: //S
			current[1][2]=3;
			current[1][3]=3;
			current[2][1]=3;
			current[2][2]=3;
			break;

		case 4: //Z
			current[1][1]=4;
			current[1][2]=4;
			current[2][2]=4;
			current[2][3]=4;
			break;

		case 5: //L
			current[1][1]=5;
			current[1][2]=5;
			current[1][3]=5;
			current[2][1]=5;
			break;

		case 6: //J
			current[1][1]=6;
			current[1][2]=6;
			current[1][3]=6;
			current[2][3]=6;
			break;

		case 7: //T
			current[1][1]=7;
			current[1][2]=7;
			current[1][3]=7;
			current[2][2]=7;
			break;
	}

	for(short y=1;y<4;++y){
		for(short x=0;x<4;++x){
			if(current[y][x]!=0 && board[currentY+y][currentX+x]!=0) return 0;
		}
	}
	return 1;
}


void fixToBoard(){
	for(short y=0;y<4;++y){
		for(short x=0;x<4;++x){
			if(current[y][x]!=0){
				board[currentY+y][currentX+x]=current[y][x];
				current[y][x]=0;
			}
		}
	}

	score += 18-drop;
	score += 3* (level-1 > _level-1 ? level-1 : _level-1);
	if(!showNext) score+=5;
}


void checkFullRows(){
	char n=0;
	for(short y=ROWS-1;y>=0;--y){
		char full=1;
		for(short x=0;x<COLUMNS;++x){
			if(board[y][x]==0){
				full=0;
				break;
			}
		}

		if(full){
			++n;
			++lines;
			if(lines<=0) _level=1;
			else if(lines>=1 && lines<=90) _level=1+((lines-1)/10);
			else if(lines>=91) _level=10;

			for(short yy=y-1;yy>=0;--yy){
				for(short x=0;x<COLUMNS;++x){
					board[yy+1][x]=board[yy][x];
				}
			}
			for(int x=0;x<COLUMNS;++x){
				board[0][x]=0;
			}
			++y;
		}
	}
}

void reset(char lev, char show_next){
	level=lev+1;
	showNext=show_next;

	for(short y=0;y<ROWS;++y){
		for(short x=0;x<COLUMNS;++x){
			board[y][x]=0;
		}
	}

	for(short y=0;y<4;++y){
		for(short x=0;x<4;++x){
			current[y][x]=0;
		}
	}

	next=rand()%7+1;
	gameOver=0;
	pause=0;
	score=0;
	lines=0;
	_level=1;

	char actualLevel= level>_level ? level : _level;
	count=50*(11-actualLevel);
}


void step(){
	if(gameOver || pause) return;
	count-=50;
	if(count>0) return;

	char actualLevel= level>_level ? level : _level;
	count=50*(11-actualLevel);

	if(current[1][2]!=0){
		if(fallingPossible()){
			++currentY;
			drop=currentY;
		}else{
			fixToBoard();
			checkFullRows();
		}
	}else{
		if(!createNewPieces()) gameOver=1;
	}
}


void rotate(){
	if(gameOver || pause || current[1][2]==0) return;

	char _current[4][4];
	char len;
	char currentType=current[1][2];

	if(currentType==1) len=1;
	else if(currentType>=2 && currentType<=4) len=2;
	else len=4;

	int _orientation=(orientation+1)%len;
	for(short y=0;y<4;++y){
		for(short x=0;x<4;++x){
			if(currentType==1)      _current[y][x]=O[_orientation][y][x];
			else if(currentType==2) _current[y][x]=I[_orientation][y][x];
			else if(currentType==3) _current[y][x]=S[_orientation][y][x];
			else if(currentType==4) _current[y][x]=Z[_orientation][y][x];
			else if(currentType==5) _current[y][x]=L[_orientation][y][x];
			else if(currentType==6) _current[y][x]=J[_orientation][y][x];
			else if(currentType==7) _current[y][x]=T[_orientation][y][x];
		}
	}

	if(!collision(currentX, currentY, _current)){
		orientation=_orientation;
		for(short y=0;y<4;++y){
			for(short x=0;x<4;++x){
				current[y][x]=_current[y][x];
			}
		}
	}
}


void down(){
	if(gameOver || pause || current[1][2]==0) return;
	drop=currentY;

	while(fallingPossible()) ++currentY;
}


void left(){
	if(gameOver || pause || current[1][2]==0) return;
	if(!collision(currentX-1, currentY, current)) --currentX;
}


void right(){
	if(gameOver || pause || current[1][2]==0) return;
	if(!collision(currentX+1, currentY, current)) ++currentX;
}


void setPause(char p){
	pause=p;
}


char isPaused(){
	return pause;
}


char isGameOver(){
	return gameOver;
}


char* getBoard(){
	return (char*)board;
}


int getScore(){
	return score;
}


char getLevel(){
	return _level>level ? _level : level;
}


char getCurrentX(){
	return currentX;
}


char getCurrentY(){
	return currentY;
}


char *getCurrentType(){
	return (char*)current;
}


char *getNext(){
	if(!showNext) return NULL;
	switch(next){
		case 1: return (char*)O[0];
		case 2: return (char*)I[0];
		case 3: return (char*)S[0];
		case 4: return (char*)Z[0];
		case 5: return (char*)L[0];
		case 6: return (char*)J[0];
		case 7: return (char*)T[0];
		default: return NULL;
	}
}


#endif

