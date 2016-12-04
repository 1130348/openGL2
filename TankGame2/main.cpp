#include <ctime>
#include "main.h"
#include "GlobalDeclarations.h"
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <iostream>       // std::cout
#include <thread>
#include <irrKlang.h>

using namespace irrklang;

#pragma warning(disable:4996)
#ifdef _WIN32
#include <GL/GLAux.h>
#endif

#pragma comment (lib,"glaux.lib")

extern "C" int read_JPEG_file(const char *, char **, int *, int *, int *);

#define NOME_TEXTURA_CHAO	"deserto.jpg"
#define NUM_TEXTURAS		1
#define ID_TEXTURA_CHAO		1

#define CHAO_DIMENSAO 70



#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#define RAD(x)          (M_PI*(x)/180)
#define GRAUS(x)        (180*(x)/M_PI)

GLboolean   DEBUG;
GLboolean   VistaTopo;
GLboolean   FPS;
GLboolean   TPS;
GLboolean   TARGET;
GLboolean   GRID;
GLboolean   AJUDA;
GLboolean   FULL;
GLboolean   FPSSHOW;
GLboolean paused;
GLboolean dead;
GLuint texName;
GLuint texture[3]; //the array for our texture
GLfloat angle = 0.0;

typedef struct {
	int		sizeX, sizeY, bpp;
	char	*data;
}JPGImage;


#define MUSICA_DISPARO "disparo.wav"

JPGImage imagem;
JPGImage imagem1;
GLuint numBalas = 5;
GLuint life = 100;
ISoundEngine* engine = createIrrKlangDevice();

int frameCount = 0;
float fps2 = 0;
int currentTime = 0, previousTime = 0;

int lastTick = clock();
int fpsLimit = 30;

int main(int argc, char** argv) {

	DEBUG = GL_TRUE;
	TPS = GL_TRUE;
	FPS = GL_FALSE;
	VistaTopo = GL_FALSE;
	GRID = GL_FALSE;
	TARGET = GL_TRUE;
	AJUDA = GL_FALSE;
	FULL = GL_FALSE;
	paused = GL_FALSE;
	dead = GL_FALSE;
	FPSSHOW = GL_FALSE;


	if (!engine)
		return 0; // error starting up the engine



				  //Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(glutGet(GLUT_SCREEN_WIDTH) - 200, glutGet(GLUT_SCREEN_HEIGHT) - 200);
	glutInitWindowPosition(GLUT_SCREEN_HEIGHT / 2, GLUT_SCREEN_WIDTH / 2);

	//Create the window
	glutCreateWindow("World of Tanks");
	initRendering();

	inicia();

	glutDisplayFunc(drawScene);
	glutTimerFunc(25, timer, 0);

	glutKeyboardFunc(handleKeypress);
	glutKeyboardUpFunc(handleKeyUp);
	glutMotionFunc(handleActiveMouse);
	glutPassiveMotionFunc(handlePassiveMouse);
	glutMouseFunc(playerFire);

	glutIgnoreKeyRepeat(true);
	if (isFullscreen) {
		glutSetCursor(GLUT_CURSOR_NONE);
	}


	glutReshapeFunc(Reshape);

	glutMainLoop();
	return 0;
}

void inicia() {
	engine->stopAllSounds();

	engine->setSoundVolume(0.2);
	engine->play2D("dese.mp3", true);
	dead = false;
	deleteTanks();

	numBalas = 5;
	numTanks = 5;
	life = 100;
	playerTank = new Tank(0.0f, 0.0f, 0.0f);
	playerTank->setHealth(playerHealth);
	srand(time(0));

	createTank(numTanks);


}

void FreeTexture(GLuint texture) {
	glDeleteTextures(3, &texture);
}

void calculateFPS()
{

	frameCount++;
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	int timeInterval = currentTime - previousTime;

	if (timeInterval > 1000)
	{

		fps2 = frameCount / (timeInterval / 1000.0f);
		previousTime = currentTime;
		frameCount = 0;
	}
}

void drawFPS()
{

	glLoadIdentity();

	string str = std::to_string(fps2);
	printtext(10, 800, str);
}



void loadTexture(GLuint texture, const char* filename)
{
	JPGImage image;
	read_JPEG_file(filename, &image.data, &image.sizeX, &image.sizeY, &image.bpp);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image.sizeX, image.sizeY, GL_RGB, GL_UNSIGNED_BYTE, image.data);
}


//Initializes 3D rendering
void initRendering() {

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearColor(fogColour[0], fogColour[1], fogColour[2], fogColour[3]);


	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_position[] = { 1.0, 1.0, 0.0 };

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);


	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	//glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING); //Enable lighting
	glEnable(GL_LIGHT0); //Enable light #0
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE); //Automatically normalize normals
	glShadeModel(GL_SMOOTH); //Enable smooth shading

	glGenTextures(3, texture);
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	loadTexture(texture[0], "deserto.jpg");
	loadTexture(texture[1], "parede.jpg");
	loadTexture(texture[2], "tanques.jpg");

	glEnable(GL_TEXTURE_2D);
}

void timer(int n) {



	calculateFPS();

	checkInput();

	if (!paused) {
		checkFire();
		slowMotionCounter++;

		if (slowMotionCounter >= slowMotionMagnitude) {
			for (unsigned int i = 0; i < tanks.size(); i++) {
				tanks[i]->move();
				tanks[i]->runTanksBots();
				slowMotionCounter = 0;
			}
		}

		if (playerTank->isDead() && (!invincibility)) {

			paused = true;
		}
		playerTank->move();

		bulletSpeed = bulletSpeedOriginal*(50.0f - slowMotionMagnitude) / 50.0f;
		lagDistance *= 0.95;
		screenShakeMagnitude *= 0.95;
		zoomMagnitude *= 0.95;


		glutPostRedisplay();

	}
	glutTimerFunc(25, timer, 0);
}

void checkFire() {

	bool bulletNotDead = true;
	for (unsigned int i = 0; i < bullets.size(); i++) {
		bullets[i]->move();
		bulletNotDead = true;
		for (unsigned int j = 0; j < tanks.size() && bulletNotDead; j++) {
			if (tanks[j]->isHitBy(bullets[i])) {
				tanks[j]->damage(30);
				bullets[i]->flagAsDead();
				bulletNotDead = false;
			}
		}
		if (bulletNotDead && playerTank->isHitBy(bullets[i])) {

			if (!invincibility) {
				if (life >= 2) {
					playerTank->damage(2);
					life -= 2;
					screenShakeMagnitude += 1.0f;
				}
				else {

					dead = true;

				}

			}
			bullets[i]->flagAsDead();
		}
	}


	for (unsigned int i = 0; i < tanks.size(); i++) {
		if (tanks[i]->isDead()) {
			delete tanks[i];
			tanks.erase(tanks.begin() + i);
			numTanks--;
		}
	}

	for (unsigned int i = 0; i < bullets.size(); i++) {
		if (bullets[i]->isDead()) {
			delete bullets[i];
			bullets.erase(bullets.begin() + i);
		}
	}
}

void checkInput() {

	if (keyDown[27]) {

		engine->drop();
		exit(0);
	}
	if (keyDown['w']) {
		playerTank->accelerate(true);
		playerTank->move();

	}

	if (keyDown['s']) {
		playerTank->accelerate(false);
		playerTank->move();
	}
	if (keyDown['a']) {
		playerTank->rotate(true);
		playerTank->rotateTurret(1.0f);
		playerTank->move();
	}
	if (keyDown['d']) {
		playerTank->rotate(false);
		playerTank->rotateTurret(-1.0f);

		playerTank->move();
	}
	if (keyDown['n']) {

		if (playerTank->giveRotationSpeed() > 0.5f) {
			playerTank->rotateTurret(1.0f);
		}
		else if (playerTank->giveRotationSpeed() < -0.5f) {
			playerTank->rotateTurret(-1.0f);
		}
		playerTank->rotateTurret(1.5f);
		lagDistance += 2.5;
	}
	if (keyDown['m']) {

		if (playerTank->giveRotationSpeed() > 0.5f) {
			playerTank->rotateTurret(1.0f);
		}
		else if (playerTank->giveRotationSpeed() < -0.5f) {
			playerTank->rotateTurret(-1.0f);
		}
		playerTank->rotateTurret(-1.5f);
		lagDistance -= 2.5;
	}

	if (keyDown['e']) {
		zoomMagnitude += 0.02;
	}
	if (keyDown['k']) {
		int direction = playerTank->centerTurret();
		if (direction > 0) {
			lagDistance += 2.5;
		}
		else if (direction < 0) {
			lagDistance -= 2.5;
		}
	}



	if (keyDown['r']) {
		paused = false;
		inicia();
	}
	if (keyDown['p']) {

		FPS = GL_TRUE;
		TPS = GL_FALSE;
		VistaTopo = GL_FALSE;

	}


	if (keyDown['t']) {


		FPS = GL_FALSE;
		TPS = GL_TRUE;
		VistaTopo = GL_FALSE;

	}

	if (keyDown['v']) {

		FPS = GL_FALSE;
		TPS = GL_FALSE;
		VistaTopo = GL_TRUE;

	}
	if (keyDown['x'] || leftMouseDown) {

		if (playerTank->fire()) {
			numBalas--;
			screenShakeMagnitude += 0.1f;
			engine->play2D("disparo.wav", false);

		}


	}

	if (keyDown['g']) {

		if (GRID) {
			GRID = GL_FALSE;
		}
		else {
			GRID = GL_TRUE;
		}
	}

	if (keyDown['u']) {


		if (TARGET) {
			TARGET = GL_FALSE;
		}
		else {
			TARGET = GL_TRUE;
		}
	}
	if (keyDown['f']) {
		if (FULL) {
			FULL = GL_FALSE;
			glutReshapeWindow(glutGet(GLUT_SCREEN_WIDTH) - 200, glutGet(GLUT_SCREEN_HEIGHT) - 200);
			glutPositionWindow(GLUT_SCREEN_HEIGHT / 2, GLUT_SCREEN_WIDTH / 2);


		}
		else {
			glutFullScreen();
			FULL = GL_TRUE;
		}


	}

	if (keyDown['h']) {

		if (AJUDA) {
			AJUDA = GL_FALSE;
			paused = !paused;

		}
		else {
			AJUDA = GL_TRUE;


		}
	}

	if (keyDown['\\']) {

		if (FPSSHOW) {
			FPSSHOW = GL_FALSE;
		}
		else {
			FPSSHOW = GL_TRUE;
		}
	}

}
void handleKeypress(unsigned char key, int x, int y) {
	keyDown[key] = true;
}

void handleKeyUp(unsigned char key, int x, int y) {

	keyDown[key] = false;

}


//Draws the 3D scene
void drawScene() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();


	float x = 0;
	float y = 0;
	float z = 0;

	if (TPS) {

		glTranslatef(0.0f, -1.5f, -3.0f);
		glRotatef(10, 1.0f, 0.0f, 0.0f);

		glTranslatef(0.0f, -0.0f, -3.0f);
		glRotatef(10, 1.0f, 0.0f, 0.0f);
	}
	else if (FPS) {

		glTranslatef(0.0f, -1.5f, 3.0f);
		glRotatef(10, 1.0f, 0.0f, 0.0f);
	}
	else if (VistaTopo) {

		gluLookAt(0.0, 20.0, 70.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0);
		glRotatef(screenShakeMagnitude, x, y, z);
	}

	glRotatef(screenShakeMagnitude, x, y, z);
	glTranslatef(0.0f, -1.5f, -6.0f);
	glRotatef(10, 1.0f, 0.0f, 0.0f);
	glRotatef(-playerTank->giveRotation(), 0.0f, 1.0f, 0.0f);
	glTranslatef(-playerTank->givePosX(), 0.0f, -playerTank->givePosZ());

	desenhaChao(mapSize, texture[0]);

	desenhaParedes(mapSize, texture[1]);
	glDisable(GL_TEXTURE_2D);

	if (GRID) {

		makeGrid(mapSize);

	}


	drawBullets();
	glEnable(GL_TEXTURE_2D);


	for (unsigned int i = 0; i < tanks.size(); i++) {

		glColor3f(1.0, 0, 1);
		tanks[i]->buildTank(texture[2]);
	}

	glPushMatrix();

	glTranslatef(playerTank->givePosX(), 1.0f, playerTank->givePosZ());
	glRotatef(playerTank->giveRotation() + playerTank->giveTurretRotation(), 0.0f, 1.0f, 0.0f);
	glColor3f(1, 0, 0);

	if (TARGET) {
		glDisable(GL_TEXTURE_2D);

		float seperation = 3.0f;

		for (int i = 1; i <= 10; i++) {
			float bulletTravel = (-seperation*i) / bulletSpeed;
			glPushMatrix();
			glTranslatef(0.0f, 0.0f, -seperation*i);
			glRotatef(-playerTank->giveRotation() - playerTank->giveTurretRotation(), 0.0f, 1.0f, 0.0f);
			glTranslatef(-playerTank->giveSpeedX()*bulletTravel, 0.0f, -playerTank->giveSpeedZ()*bulletTravel);
			glutSolidSphere(0.05f, 4, 4);
			glPopMatrix();
		}
		glEnable(GL_TEXTURE_2D);

	}

	glPopMatrix();
	glColor3f(0, 2.0f, 0.3f);
	glDisable(GL_TEXTURE_2D);
	playerTank->buildTank(texture[2]);

	glPopMatrix();

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	if (tanks.size() == 0) {
		glColor3f(0, 0, 0);
		//engine->stopAllSounds();

		printtext(520, 150, "YOU WIN!");
		printtext(520, 100, "Press R to Play Again");
		paused = true;
	}

	if (dead) {

		glColor3f(0, 0, 0);
		//engine->stopAllSounds();
		printtext(520, 150, "GAMEOVER!");
		printtext(520, 100, "Press R to Play Again");
		paused = true;
	}

	if (FPSSHOW) {
		drawFPS();
	}


	if (AJUDA) {

		glColor3f(0, 0, 0);

		printtext(520, 650, "Controlos:");
		printtext(520, 600, "h,H - Ajuda ");
		printtext(520, 550, "P   - Vista Primeira Pessoa");
		printtext(520, 500, "T   - Vista De Topo");
		printtext(520, 450, "V   - Vista Terceira Pessoa");
		printtext(520, 400, "G   - Grid ON/OFF");
		printtext(520, 350, "U   - Target ON/OFF");
		printtext(520, 300, "\\   - FPS ON/OFF");
		printtext(520, 250, "F   - Fullscreen ON/OFF");
		printtext(520, 200, "R   - Reiniciar");
		printtext(520, 150, "ESC - Sair");
		paused = !paused;

	}

	glColor3f(1, 0, 0);
	printtext(50, 50, "HEALTH:" + std::to_string(life) + "%");

	if (!playerTank->rel) {
		printtext(50, 30, "A recarregar...");
		numBalas = 5;

	}
	else {
		printtext(50, 30, std::to_string(numBalas) + "/5 RELOAD");
	}

	glEnable(GL_LIGHTING);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glutSwapBuffers();
}

void printtext(int x, int y, string string)
{
	//(x,y) is from the bottom left of the window
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, screenWidth, 0, screenHeight, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glPushAttrib(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);
	glRasterPos2i(x, y);
	for (unsigned int i = 0; i<string.size(); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
	glPopAttrib();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void imprime_ajuda(void)
{
	printf("\n\nControlos\n");
	printf("h,H - Ajuda \n");
	printf("P   - Vista Primeira Pessoa\n");
	printf("T   - Vista De Topo\n");
	printf("V   - Vista Terceira Pessoa\n");
	printf("G   - Grid ON/OFF \n");
	printf("U   - Target ON/OFF \n");
	printf("ESC - Sair\n");

}

void setOrthographicProjection() {

	// switch to projection mode
	glMatrixMode(GL_PROJECTION);
	// save previous matrix which contains the 
	//settings for the perspective projection
	glPushMatrix();
	// reset matrix
	glLoadIdentity();
	// set a 2D orthographic projection
	gluOrtho2D(0, 1440, 0, 900);
	// invert the y axis, down is positive
	glScalef(1, -1, 1);
	// mover the origin from the bottom left corner
	// to the upper left corner
	glTranslatef(0, -900, 0);
	glMatrixMode(GL_MODELVIEW);
}

void resetPerspectiveProjection() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void handlePassiveMouse(int x, int y) {
	playerTank->rotateTurret(0.1*(lastMouseX - x));
	lagDistance += 0.1*(lastMouseX - x);
	lastMouseX = x;
	if (x <= 2 || x >= 1438) {
		glutWarpPointer(770, 450);
		if (leftMouseDown) {
			if (playerTank->fire()) {
				numBalas--;
				screenShakeMagnitude += 0.1f;
			}
		}
		lastMouseX = 770;
	}
}

void playerFire(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			leftMouseDown = true;
		}
		else {
			leftMouseDown = false;
		}

	}
}

void handleActiveMouse(int x, int y) {
	handlePassiveMouse(x, y);
}


void Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)width / (double)height, 1.0, 200.0);
	screenWidth = width;
	screenHeight = height;
}


void createTank(int numTanks) {

	for (int i = 0; i < numTanks; i++) {
		int x = 0;
		int z = 0;
		while (distanceBetween(0, 0, x, z) < 20) {
			x = rand() % (2 * mapSize) - mapSize;
			z = rand() % (2 * mapSize) - mapSize;
		}
		tanks.push_back(new Tank(x, z, 0.0f));//Adiciona ultima posicao do vetor de tanques
		if (!tanks[i]->canMoveTo(tanks[i]->givePosX(), tanks[i]->givePosZ())) {
			delete tanks[i];
			tanks.erase(tanks.begin() + i);
			i--;
		}
	}

}

void deleteTanks() {

	glClear(GL_COLOR_BUFFER_BIT);
	tanks.clear();


}

void drawBullets() {

	for (unsigned int i = 0; i <bullets.size(); i++) {
		bullets[i]->drawBullet();
	}
}



void cleanBullets() {

	for (unsigned int i = 0; i < bullets.size(); i++) {
		if (bullets[i]->isDead()) {
			delete bullets[i];
			bullets.erase(bullets.begin() + i);
		}
	}

}


