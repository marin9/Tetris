#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include "graphic.h"
#include "tetris.h"
#include "controls.h"


#define EXIT	0
#define MENU	1
#define PLAY	2

#define FRAME_TIME	50

static char scale;
static char soundOn;
static char gameStatus;
static char showNext;
static char level;
static int highScore;


void loadHighScore(){
	FILE *file=fopen("data.bin", "r");
	if(file!=NULL){
		fscanf(file, "%d", &highScore);
		fclose(file);
	}
}

void storeHighScore(){
	FILE *file=fopen("data.bin", "w");
	fprintf(file, "%d", highScore);
	fclose(file);
}

char c_getch(){
	struct termios b;
	struct termios a;
	char c;

	tcgetattr(STDIN_FILENO, &b);
	a = b;
	a.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &a);
	c = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &b);

	return c;
}

char c_kbhit(void){
	char c;
	int f;

	f = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, f | O_NONBLOCK);
	c = c_getch();
	fcntl(STDIN_FILENO, F_SETFL, f);

	if (c != EOF) {
		ungetc(c, stdin);
		return 1;
	}
	return 0;
}

long getCurrentMS(){
    struct timespec c_time;

    clock_gettime(CLOCK_REALTIME, &c_time);

    return c_time.tv_nsec/1000000;	
}

void getInput(){
	char input;
	
	if(c_kbhit()){
		input=c_getch();			
		if(input>='a' && input<='z') input-=32;
			
		if(gameStatus==MENU){
			switch(input){
				case QUIT:
					gameStatus=EXIT;
					break;
					
				case LEVEL:
					level=(++level)%10;
					break;
					
				case SHNEXT:
					showNext=1-showNext;
					break;
					
				case START:
					gameStatus=PLAY;
					reset(level, showNext);
					break;
				
				default:
					break;
			}
		}else if(gameStatus==PLAY){
			switch(input){
				case QUIT:
					gameStatus=MENU;
					if(highScore<getScore()){
						highScore=getScore();
						storeHighScore();
					}
					break;
					
				case PAUSE:
					setPause(!isPaused());
					break;
					
				case ROTATE:
					rotate();
					break;
					
				case DROP:
					down();
					break;
					
				case LEFT:
					left();
					break;
				
				case RIGHT:
					right();
					break;
					
				default:
					break;
			}
		}
	}
}	


void render(){	
	if(gameStatus==MENU) renderMenu(highScore, level, showNext);
	else if(gameStatus==PLAY) renderGame(highScore);
}



int main(){
	scale=3;
	level=0;
	showNext=1;
	soundOn=0;
	gameStatus=MENU;

	srand(time(NULL));
	loadHighScore();

	while(gameStatus){
		long time=getCurrentMS();

        getInput();
		step();
		render();
		
		long sleep=FRAME_TIME-(getCurrentMS()-time); 
		//printf("%ld ms\n", (getCurrentMS()-time));  
		if(sleep<0) continue;
		else usleep(sleep*1000);
    }
	return 0;
}


