#include "Definitions.h"
#include "Cutils.h"
#include "Tank.h"


#ifndef _MAIN_H
#define _MAIN_H

void handleKeypress(unsigned char key, int x, int y);
void handleSpecialKeypress(int key, int x, int y);
void handleResize(int w, int h);
void handlePassiveMouse(int x, int y);
void handleActiveMouse(int x, int y);


void initRendering();
void drawScene();
void setOrthographicProjection();
void resetPerspectiveProjection();
void renderBitmapString(float x, float y, float z, void *font, char *string);
void timer(int value);
int main(int argc, char** argv);
void makeTank();
void handleKeyUp(unsigned char key, int x, int y);
void keyBoard();
void playerFire(int button, int state, int x, int y);
void createTank(float x, float y);
void createObstacle(float x, float z, float r);
void drawHealthBars();

#endif






