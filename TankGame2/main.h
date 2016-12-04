#include <iostream>
#include <string.h>
#include <string>
#include "Definitions.h"
#include "MathForms.h"
#include "Tank.h"
#include "Bullet.h"

using namespace std;

#ifndef _MAIN_H
#define _MAIN_H

bool handleKeypress(unsigned char key);
void Reshape(int w, int h);
void handlePassiveMouse(int x, int y);
void handleActiveMouse(int x, int y);

void printtext(int x, int y, std::string string);

void initRendering();
void drawScene();
void inicia();
void deleteTanks();
void setOrthographicProjection();
void resetPerspectiveProjection();
void timer(int n);
int main(int argc, char** argv);
//void makeTank();
void handleKeypress(unsigned char key, int x, int y);
void handleKeyUp(unsigned char key, int x, int y);
void checkInput();
void playerFire(int button, int state, int x, int y);
void createTank(int numTanks);
void checkBullets();
void drawBullets();
void cleanBullets();
void checkFire();





#endif






