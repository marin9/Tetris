#include <SDL2/SDL.h>
#include <time.h>
#include "graphic.h"
#include "audio.h"
#include "tetris.h"

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


void getInput(){
	SDL_Event event;

	while(SDL_PollEvent(&event)){
        if(event.type==SDL_QUIT) gameStatus=EXIT;
		else if(event.type==SDL_KEYDOWN && gameStatus==MENU){
			int key=event.key.keysym.sym;

			if(key==SDLK_RETURN){
				gameStatus=PLAY;
				reset(level, showNext);
				setMusic(soundOn);

			}else if(key==SDLK_r){
				scale=(++scale)%5;
				if(scale==0) scale=1;
				setResolution(scale);
			}
			else if(key==SDLK_s){
				soundOn=!soundOn;

			}else if(key==SDLK_ESCAPE) gameStatus=EXIT;
			else if(key==SDLK_RIGHT || key==SDLK_LCTRL) level=(++level)%10;
			else if(key==SDLK_LEFT) showNext=!showNext;

		}else if(event.type==SDL_KEYDOWN && gameStatus==PLAY){
			int key=event.key.keysym.sym;

			if(key==SDLK_ESCAPE){
				gameStatus=MENU;
				if(highScore<getScore()){
					highScore=getScore();
					storeHighScore();
				}
				setMusic(0);
			}else if(key==SDLK_s){
				soundOn=!soundOn;
				setMusic(soundOn);

			}else if(key==SDLK_p){
				setPause(!isPaused());
				pauseMusic(isPaused() || !soundOn);
			}else if(key==SDLK_UP || key==SDLK_LCTRL) rotate();
			else if(key==SDLK_DOWN) down();
			else if(key==SDLK_LEFT) left();
			else if(key==SDLK_RIGHT) right();
		}
    }
}


void render(){	
	if(gameStatus==MENU) renderMenu(highScore, level, showNext, soundOn);
	else if(gameStatus==PLAY) renderGame(highScore, soundOn);
}



int main(){
	scale=3;
	level=0;
	showNext=1;
	soundOn=0;
	gameStatus=MENU;

	initGraphic(scale);
	initAudio();
	srand(time(NULL));
	loadHighScore();

	while(gameStatus){
		int time=SDL_GetTicks();

        getInput();
		step();
		render();
	
		int sleep=FRAME_TIME-(SDL_GetTicks()-time);   
		//printf("%d ms\n", (SDL_GetTicks()-time));
		SDL_Delay(sleep);
    }

	cleanGraphic();
	cleanAudio();
	SDL_Quit();
	return 0;
}


