#include <SDL2/SDL.h>
#include "graphic.h"
#include "tetris.h"

#define BLACK	0
#define YELLOW	1
#define CYAN	2
#define GREEN	3
#define RED		4
#define ORANGE	5
#define BLUE	6
#define PURPLE	7
#define GRAY	8


static SDL_Color const color[9]={{0,0,0,0},
								{255,255,0,0},
								{0,255,255,0},
								{0,255,0,0},
								{255,0,0,0},
								{255,170,0,0},
								{0,0,255,0},
								{255,0,255,0},
								{128,128,128,0}};
static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *panelTexture;
static SDL_Texture *textImage;
static int scale;



void initGraphic(int x_scale){
	SDL_Init(SDL_INIT_VIDEO);

	scale=x_scale;
	window=SDL_CreateWindow("Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 200*scale, 220*scale, SDL_WINDOW_OPENGL);
	if(window==NULL){
		printf("ERROR: Could not create window: %s\n", SDL_GetError());
		exit(1);
	}

	renderer=SDL_CreateRenderer(window, -1, 0);
	panelTexture=SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, 200, 220);
	
	SDL_Surface *bitmap=SDL_LoadBMP("res/text.bmp");
	if(bitmap==NULL){
		printf("ERROR: Could not found text.bmp resource\n");
		exit(2);
	}

	textImage=SDL_CreateTextureFromSurface(renderer, bitmap);
	SDL_FreeSurface(bitmap);
}


void setResolution(char sc){
	scale=sc;
	SDL_SetWindowSize(window, 200*scale, 220*scale);
}


void drawNumber(int num, int x, int y){
	SDL_Rect srect={0, 40, 10, 16};
	SDL_Rect drect={x, y, 10, 16};

	char c=0;
	for(int i=1000000;i>=10;i=i/10){
		int digit=(num/i)%10;
		if(digit) c=1;
		srect.x=digit*10;
		if(c) SDL_RenderCopy(renderer, textImage, &srect, &drect);
		drect.x+=10;
	}

	srect.x=(num%10)*10;
	SDL_RenderCopy(renderer, textImage, &srect, &drect);
}


void drawRect(int x, int y, int w, int h, int c){
	SDL_Rect rect={x, y, w, h};
	SDL_SetRenderDrawColor(renderer, color[c].r, color[c].g, color[c].b, 0);
	SDL_RenderFillRect(renderer, &rect);
}


void renderMenu(int hi_sc, char level, char showNext, char soundOn){
	SDL_SetRenderTarget(renderer, panelTexture);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	//draw board
	drawRect(9, 9, 102, 202, GRAY);
	drawRect(10, 10, 100, 200, BLACK);
	SDL_Rect sr={0, 65, 97, 160};
	SDL_Rect dr={11, 10, 97, 160};
	SDL_RenderCopy(renderer, textImage, &sr, &dr);

	//draw Hi-Score
	SDL_Rect srect={0, 0, 60, 10};
	SDL_Rect drect={120, 10, 60, 10};
	SDL_RenderCopy(renderer, textImage, &srect, &drect);
	drawNumber(hi_sc, 120, 22);

	//draw Score
	srect.y=10;
	drect.y=40;
	SDL_RenderCopy(renderer, textImage, &srect, &drect);

	//draw Next
	srect.y=20;
	drect.y=70;
	SDL_RenderCopy(renderer, textImage, &srect, &drect);
	if(showNext){
		drawRect(120+10, 80+10, 30, 10, PURPLE);
		drawRect(120+20, 80+20, 10, 10, PURPLE);
	}

	//draw Level
	srect.y=30;
	drect.y=120;
	SDL_RenderCopy(renderer, textImage, &srect, &drect);
	drawNumber(level+1, 120, 130);

	if(soundOn){
		SDL_Rect sr={60, 0, 30, 34};
		SDL_Rect dr={120, 150, 30, 34};
		SDL_RenderCopy(renderer, textImage, &sr, &dr);
	}


	SDL_SetRenderTarget(renderer, NULL);
	SDL_Rect rr={0, 0, 200*scale, 220*scale};
	SDL_RenderCopy(renderer, panelTexture, NULL, &rr);
	SDL_RenderPresent(renderer);
}


void renderGame(int hi_sc, char soundOn){
	char *board=getBoard();
	char *next=getNext();
	char *current=getCurrentType();
	char cx=getCurrentX();
	char cy=getCurrentY();

	SDL_SetRenderTarget(renderer, panelTexture);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
		
	//draw board
	drawRect(9, 9, 102, 202, GRAY);
	drawRect(10, 10, 100, 200, BLACK);
	for(int y=0;y<20;++y){
		for(int x=0;x<10;++x){
			if(*(board+x+y*10)==0) drawRect(15+10*x, 15+10*y, 1, 1, GRAY);
			else drawRect(10+10*x, 10+10*y, 10, 10, *(board+x+y*10));
		}
	}

	//draw current
	for(int y=0;y<4;++y){
		for(int x=0;x<4;++x){
			if(*(current+x+y*4)) drawRect(10+10*(x+cx), 10+10*(y+cy), 10, 10, *(current+x+y*4));
		}
	}

	//draw Hi-Score
	SDL_Rect srect={0, 0, 60, 10};
	SDL_Rect drect={120, 10, 60, 10};
	SDL_RenderCopy(renderer, textImage, &srect, &drect);
	drawNumber(hi_sc, 120, 22);

	//draw Score
	srect.y=10;
	drect.y=40;
	SDL_RenderCopy(renderer, textImage, &srect, &drect);
	drawNumber(getScore(), 120, 52);

	//draw Next
	srect.y=20;
	drect.y=70;
	SDL_RenderCopy(renderer, textImage, &srect, &drect);
	if(next!=NULL){
		for(int y=0;y<4;++y){
			for(int x=0;x<4;++x){
				drawRect(120+10*x, 80+10*y, 10, 10, *(next+x+y*4));
			}
		}
	}

	//draw Level
	srect.y=30;
	drect.y=120;
	SDL_RenderCopy(renderer, textImage, &srect, &drect);
	drawNumber(getLevel(), 120, 130);

	if(soundOn){
		SDL_Rect sr={60, 0, 30, 34};
		SDL_Rect dr={120, 150, 30, 34};
		SDL_RenderCopy(renderer, textImage, &sr, &dr);
	}

	if(isPaused()){
		SDL_Rect sr={100, 0, 30, 34};
		SDL_Rect dr={160, 150, 30, 34};
		SDL_RenderCopy(renderer, textImage, &sr, &dr);
	}

	if(isGameOver()){
		SDL_Rect sr={100, 200, 30, 20};
		SDL_Rect dr={137, 190, 30, 20};
		SDL_RenderCopy(renderer, textImage, &sr, &dr);
	}


	SDL_SetRenderTarget(renderer, NULL);
	SDL_Rect rr={0, 0, 200*scale, 220*scale};
	SDL_RenderCopy(renderer, panelTexture, NULL, &rr);
	SDL_RenderPresent(renderer);
}


void cleanGraphic(){
	SDL_DestroyTexture(textImage);
	SDL_DestroyTexture(panelTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}


