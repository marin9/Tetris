#include <SDL2/SDL.h>
#include "graphic.h"
#include "tetris.h"

#define WIDTH	200
#define HEIGHT	220

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
static SDL_Texture *gameImage;
static int scale;



void initGraphic(int x_scale){
	SDL_Init(SDL_INIT_VIDEO);
	scale=x_scale;

	//Create window
	window=SDL_CreateWindow("Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH*scale, HEIGHT*scale, SDL_WINDOW_OPENGL);
	if(window==NULL){
		printf("ERROR: Could not create window: %s\n", SDL_GetError());
		exit(1);
	}

	//Create renderer
	renderer=SDL_CreateRenderer(window, -1, 0);

	//Create texture(window panel)
	panelTexture=SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, WIDTH, HEIGHT);

	SDL_Surface *bitmap=SDL_LoadBMP("res/game.bmp");
	if(bitmap==NULL){
		printf("ERROR: Could not found game.bmp resource\n");
		exit(2);
	}

	gameImage=SDL_CreateTextureFromSurface(renderer, bitmap);
	SDL_FreeSurface(bitmap);
}


void setResolution(char sc){
	scale=sc;
	SDL_SetWindowSize(window, WIDTH*scale, HEIGHT*scale);
}


void drawNumber(int num, int x, int y){
	SDL_Rect srect={10, 195, 10, 14};
	SDL_Rect drect={x, y, 10, 14};

	char c=0;
	for(int i=1000000; i>=10; i=i/10){
		int digit=(num/i)%10;
		if(digit) c=1;
		srect.x=10+digit*10;
		if(c) SDL_RenderCopy(renderer, gameImage, &srect, &drect);
		drect.x+=10;
	}

	srect.x=10+(num%10)*10;
	SDL_RenderCopy(renderer, gameImage, &srect, &drect);
}


void drawRect(int x, int y, int w, int h, int c){
	SDL_Rect rect={x, y, w, h};
	SDL_SetRenderDrawColor(renderer, color[c].r, color[c].g, color[c].b, 0);
	SDL_RenderFillRect(renderer, &rect);
}


void renderMenu(int hi_sc, char level, char showNext, char soundOn){
	SDL_SetRenderTarget(renderer, panelTexture);

	//draw board
	SDL_RenderCopy(renderer, gameImage, NULL, NULL);
	drawRect(10, 195, 100, 14, BLACK);

	//draw Hi-Score
	drawNumber(hi_sc, 120, 22);

	//draw Next
	if(showNext){
		drawRect(130+10, 80+10, 30, 10, PURPLE);
		drawRect(130+20, 80+20, 10, 10, PURPLE);
	}

	//draw Level
	drawNumber(level+1, 120, 130);

	//sound icon
	if(!soundOn){
		drawRect(120, 170, 25, 26, BLACK);
	}

	drawRect(160, 170, 25, 26, BLACK);

	drawRect(120, 200, 70, 10, BLACK);

	SDL_SetRenderTarget(renderer, NULL);
	SDL_Rect rr={0, 0, WIDTH*scale, HEIGHT*scale};
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

	//draw board
	SDL_RenderCopy(renderer, gameImage, NULL, NULL);
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
	drawNumber(hi_sc, 120, 22);

	//draw Score
	drawNumber(getScore(), 120, 52);

	//draw Next
	if(next!=NULL){
		for(int y=0;y<4;++y){
			for(int x=0;x<4;++x){
				drawRect(130+10*x, 80+10*y, 10, 10, *(next+x+y*4));
			}
		}
	}

	//draw Level
	drawNumber(getLevel(), 120, 130);

	if(!soundOn){
		drawRect(120, 170, 25, 26, BLACK);
	}

	if(!isPaused()){
		drawRect(160, 170, 25, 26, BLACK);
	}

	if(!isGameOver()){
		drawRect(120, 200, 70, 10, BLACK);
	}

	SDL_SetRenderTarget(renderer, NULL);
	SDL_Rect rr={0, 0, WIDTH*scale, HEIGHT*scale};
	SDL_RenderCopy(renderer, panelTexture, NULL, &rr);
	SDL_RenderPresent(renderer);
}


void cleanGraphic(){
	SDL_DestroyTexture(gameImage);
	SDL_DestroyTexture(panelTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}


