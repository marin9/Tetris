#ifndef GRAPHIC_H
#define GRAPHIC_H

void initGraphic(int x_scale);
void setResolution(char sc);
void drawNumber(int num, int x, int y);
void drawRect(int x, int y, int w, int h, int c);
void renderMenu(int hi_sc, char level, char showNext, char soundOn);
void renderGame(int hi_sc, char soundOn);
void cleanGraphic();

#endif

