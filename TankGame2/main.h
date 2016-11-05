#include "Definitions.h"
#include "Cutils.h"
#include "Tank.h"


#ifndef _MAIN_H
#define _MAIN_H

void handleKeypress(unsigned char key, int x, int y);
void SpecialKey(int key, int x, int y);
void Reshape(int width, int height);
void handlePassiveMouse(int x, int y);
void handleActiveMouse(int x, int y);


void initRendering();
void drawScene();
void setOrthographicProjection();
void resetPerspectiveProjection();
void renderBitmapString(float x, float y, float z, void *font, char *string);
void imprime_ajuda(void);
void timer(int n);
int main(int argc, char** argv);

void KeyUp(unsigned char key, int x, int y);
void Key(unsigned char key, int x, int y);
void playerFire(int button, int state, int x, int y);
void createTank(float x, float y);
void createObstacle(float x, float z, float r);
void drawHealthBars();

#endif






