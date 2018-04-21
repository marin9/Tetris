#include <stdio.h>
#include "graphic.h"
#include "tetris.h"
#include "controls.h"


#define NORMAL	"\x1B[1;0m"
#define RED	"\x1B[1;31m"
#define GREEN	"\x1B[1;32m"
#define YELLOW	"\x1B[1;33m"
#define BLUE	"\x1B[1;34m"
#define MAGENTA	"\x1B[1;35m"
#define CYAN	"\x1B[1;36m"
#define WHITE	"\x1B[1;37m"

#define D_NORMAL	"\x1B[0;0m"
#define D_RED	"\x1B[0;31m"
#define D_GREEN	"\x1B[0;32m"
#define D_YELLOW	"\x1B[0;33m"
#define D_BLUE	"\x1B[0;34m"
#define D_MAGENTA	"\x1B[0;35m"
#define D_CYAN	"\x1B[0;36m"
#define D_WHITE	"\x1B[0;37m"

#define B_BLACK		"\x1B[1;40m"
#define B_RED		"\x1B[1;41m"
#define B_GREEN		"\x1B[1;42m"
#define B_YELLOW	"\x1B[1;43m"
#define B_BLUE		"\x1B[1;44m"
#define B_MAGENTA	"\x1B[1;45m"
#define B_CYAN		"\x1B[1;46m"
#define B_WHITE		"\x1B[1;47m"

#define CLEAR	"\033[H\033[J" NORMAL


void renderMenu(int hi_sc, char level, char showNext){
	printf("%s", CLEAR);
	
	printf("\n\n\t%s%c %s%c %s%c %s%c %s%c %s%c\n\n", 
			MAGENTA, 'T', YELLOW, 'E', BLUE, 'T', 
			GREEN, 'R', RED, 'I', CYAN, 'S');
			
	printf("\t%sHigh score: \t%d\n\n", BLUE, hi_sc);
	printf("\t%sStart (Enter)\n\n", GREEN);
	printf("\t%sLevel (L): \t%d\n\n", MAGENTA, level+1);
	printf("\t%sShow next(N): \t%s\n\n", YELLOW, (showNext? "On":"Off"));
	printf("\t%sExit (Esc)\n\n", RED);
	
	printf("%s", NORMAL);
	printf("\tROTATE:\t%c\n", ROTATE);
	printf("\tLEFT:\t%c\n", LEFT);
	printf("\tRIGHT:\t%c\n", RIGHT);
	printf("\tDROP:\t%c\n", DROP);
	printf("\tPAUSE:\t%c\n", PAUSE);
	
	fflush(stdout);
}


void renderGame(int hi_sc){
	int x, y;
	char *board=getBoard();
	char *next=getNext();
	char *current=getCurrentType();
	char cx=getCurrentX();
	char cy=getCurrentY();
	char tmp_board[ROWS*COLUMNS]={0};
	
	for(y=0;y<ROWS;++y){
		for(x=0;x<COLUMNS;++x){
			tmp_board[y*COLUMNS+x]=board[y*COLUMNS+x];
		}
	}
	for(x=0;x<4;++x){
		for(y=0;y<4;++y){
			if(current[y*4+x]){
				tmp_board[x+cx+(y+cy)*COLUMNS]=current[y*4+x];
			}
		}
	}
	
	printf("%s", CLEAR);
	printf("\tHi-Score:\t%d\n", hi_sc);
	printf("\tScore:\t\t%d\n", getScore());
	printf("\tLevel:\t\t%d\n", getLevel());
	
	//draw next
	if(next!=NULL){
		switch(next[1*4+2]){
			case 1:
				printf("\tNext:\t\t%s%c", D_BLUE, 'O');
				break;
			
			case 2:
				printf("\tNext:\t\t%s%c", D_RED, 'I');
				break;
				
			case 3:
				printf("\tNext:\t\t%s%c", D_GREEN, 'S');
				break;
			
			case 4:
				printf("\tNext:\t\t%s%c", D_CYAN, 'Z');
				break;
				
			case 5:
				printf("\tNext:\t\t%s%c", D_MAGENTA, 'L');
				break;
				
			case 6:
				printf("\tNext:\t\t%s%c", D_WHITE, 'J');
				break;
				
			case 7:
				printf("\tNext:\t\t%s%c", D_YELLOW, 'T');
				break;
			
			default:
				break;
		}
		printf("\n");
	}else{
		printf("\tNext:\t\t-\n");
	}
	
	
	if(isGameOver()) printf("%s\n\t\tGAME OVER\n\n", RED);
	else if(isPaused()) printf("%s\n\t\tPAUSE\n\n", GREEN);
	else printf ("\n\n\n");
	
	//draw board
	for(y=0;y<ROWS;++y){
		printf("%s\t#", BLUE);
		for(x=0;x<COLUMNS;++x){
			switch(tmp_board[y*COLUMNS+x]){
				case 1:
					printf("%s  %s", B_BLUE, B_BLACK);
					break;
					
				case 2:
					printf("%s  %s", B_RED, B_BLACK);
					break;
					
				case 3:
					printf("%s  %s", B_GREEN, B_BLACK);
					break;
				
				case 4:
					printf("%s  %s", B_CYAN, B_BLACK);
					break;
					
				case 5:
					printf("%s  %s", B_MAGENTA, B_BLACK);
					break;
					
				case 6:
					printf("%s  %s", B_WHITE, B_BLACK);
					break;
					
				case 7:
					printf("%s  %s", B_YELLOW, B_BLACK);
					break;
				
				default:
					if(x%2!=0) printf("%s %s.%s", B_BLACK, BLUE, B_BLACK);
					else printf("%s  ", B_BLACK);
					break;
			}
		}
		printf("%s#\n", BLUE);
	}
	printf("\t");
	for(x=0;x<(COLUMNS+1)*2;++x) printf("#");
	printf("%s\n", NORMAL);
	
	fflush(stdout);
}
	
