#include <ctime>
#include "main.h"
#include "GlobalDeclarations.h"

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


int main(int argc, char** argv) {

	DEBUG = GL_TRUE;
	TPS = GL_TRUE;
	FPS = GL_FALSE;
	VistaTopo = GL_FALSE;
	GRID = GL_TRUE;
	TARGET = GL_TRUE;


	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(glutGet(GLUT_SCREEN_WIDTH) - 200, glutGet(GLUT_SCREEN_HEIGHT) - 200);
	glutInitWindowPosition(GLUT_SCREEN_HEIGHT/2, GLUT_SCREEN_WIDTH / 2);
	
	//Create the window
	glutCreateWindow("World of Tanks");
	initRendering();


	
	//Set handler functions
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutKeyboardUpFunc(handleKeyUp);
	glutMotionFunc(handleActiveMouse);
	glutPassiveMotionFunc(handlePassiveMouse);
	glutReshapeFunc(Reshape);
	glutIgnoreKeyRepeat(true);
	if (isFullscreen){
		glutSetCursor(GLUT_CURSOR_NONE);
	}
	glutMouseFunc(playerFire);
	glutIgnoreKeyRepeat(1);
	
	inicia();
	
	glutTimerFunc(25, timer, 0);
	glutMainLoop();
	return 0;
}

void inicia() {

	deleteTanks();
	playerTank = new Tank(0.0f, 0.0f, 0.0f);
	playerTank->setHealth(playerHealth);
	srand(time(0));

	createTank(numTanks);

}

//Initializes 3D rendering
void initRendering() {
	
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearColor(fogColour[0], fogColour[1], fogColour[2], fogColour[3]);

	/*glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);*/
	// set the fog attributes
	/*glFogf(GL_FOG_START,  1.0f);
	glFogf(GL_FOG_END,    200.0f);
	glFogfv(GL_FOG_COLOR,  fogColour);
	glFogi(GL_FOG_MODE,   GL_EXP);
	glFogf(GL_FOG_DENSITY, 0.01f);*/
	
	// enable the fog
	//glEnable(GL_FOG);
	
	/*glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING); //Enable lighting
	glEnable(GL_LIGHT0); //Enable light #0
	glEnable(GL_LIGHT1); //Enable light #1
	glEnable(GL_NORMALIZE); //Automatically normalize normals
	glShadeModel(GL_SMOOTH); //Enable smooth shading*/
	
	
}

void timer(int n) {

	checkInput();
	
	slowMotionCounter++;
	if (slowMotionCounter >= slowMotionMagnitude) {
		for (int i = 0; i < tanks.size(); i++) {
			tanks[i]->move();
			tanks[i]->runAI();
			slowMotionCounter = 0;
		}
	}

	playerTank->move();
	glutPostRedisplay();
	glutTimerFunc(25, timer, 0);
}

void checkInput() {
	
	if (keyDown[27]) {
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
		playerTank->move();
	}
	if (keyDown['d']) {
		playerTank->rotate(false);
		playerTank->move();
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


	
}

void handleKeypress(unsigned char key, int x, int y) {    
	keyDown[key] = true;
	//printf("Carregou na tecla %c\n", key);
}

void handleKeyUp(unsigned char key, int x, int y) {
	keyDown[key] = false;
	//printf("Carregou na tecla %c\n", key);
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
			//printf("TPS \n");


			glTranslatef(0.0f, -1.5f, -3.0f);
			glRotatef(10, 1.0f, 0.0f, 0.0f);

			glTranslatef(0.0f, -0.0f, -3.0f);
			glRotatef(10, 1.0f, 0.0f, 0.0f);
		}
		else if (FPS) {
			//printf("FPS \n");

			glTranslatef(0.0f, -1.5f, 3.0f);
			glRotatef(10, 1.0f, 0.0f, 0.0f);
		}
		else if (VistaTopo) {
			//printf("VP \n");
			gluLookAt(0.0, 20.0, 20.0, 0.0, 0.0,0.0, 0.0, 0.0, -1.0);
			glRotatef(screenShakeMagnitude, x, y, z);
		}

		glRotatef(screenShakeMagnitude, x, y, z);
		glTranslatef(0.0f, -1.5f, -6.0f);
		glRotatef(10, 1.0f, 0.0f, 0.0f);
		glRotatef(-playerTank->giveRotation(), 0.0f, 1.0f, 0.0f);
		glTranslatef(-playerTank->givePosX(), 0.0f, -playerTank->givePosZ());


		if (GRID) {
			makeGrid(mapSize);
		}

		for (int i = 0; i < tanks.size(); i++) {

			glColor3f(1.0, 0, 1);
			tanks[i]->drawSelf();
		}
		


		glPushMatrix();

			glTranslatef(playerTank->givePosX(), 1.0f, playerTank->givePosZ());
			glRotatef(playerTank->giveRotation() + playerTank->giveTurretRotation(), 0.0f, 1.0f, 0.0f);
			glColor3f(1,0 , 0);

			if (TARGET) {
				float seperation = 1.0f;

				for (int i = 1; i <= 10; i++) {
					float bulletTravel = (-seperation*i) / bulletSpeed;
					glPushMatrix();
						glTranslatef(0.0f, 0.0f, -seperation*i);
						glRotatef(-playerTank->giveRotation() - playerTank->giveTurretRotation(), 0.0f, 1.0f, 0.0f);
						glTranslatef(-playerTank->giveSpeedX()*bulletTravel, 0.0f, -playerTank->giveSpeedZ()*bulletTravel);
						glutSolidSphere(0.1, 4, 4);
					glPopMatrix();
				}
			}
		glPopMatrix();
		glColor3f(0, 2.0f, 0.3f);
		playerTank->drawSelf();

	glPopMatrix();
	
	//glDisable(GL_LIGHTING);
	//drawHealthBars();
	//glEnable(GL_LIGHTING);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1, 0, 0);
	printtext(50, 50, "HEALTH: 100%");
	printtext(50, 35, "1/5 RELOAD");

	glutSwapBuffers();
}

void renderBitmapString(float x, float y, float z, void *font, char *string) {  

	char *c;
	glRasterPos3f(x, y,z);
	for (c=string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}

void printtext(int x, int y,   string string)
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
	for (int i = 0; i<string.size(); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string[i]);
	}
	glPopAttrib();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void imprime_ajuda(void)
{
	printf("\n\nDesenho de um quadrado\n");
	printf("h,H - Ajuda \n");
	printf("P   - Vista Primeira Pessoa\n");
	printf("T   - Vista De Topo\n");
	printf("V   - Vista Terceira Pessoa\n");
	printf("G   - Grid ON/OFF \n");
	printf("U   - Target ON/OFF \n");
	printf("ESC - Sair\n");
	printf("teclas do rato para iniciar/parar rotação e alternar eixos\n");

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

void handlePassiveMouse(int x, int y){
	playerTank->rotateTurret(0.1*(lastMouseX - x));
	lagDistance += 0.1*(lastMouseX - x);
	lastMouseX = x;
	if (x <= 2 || x >= 1438){
		glutWarpPointer(770, 450);
		if (leftMouseDown){
			playerFire(GLUT_LEFT_BUTTON, GLUT_DOWN, 0, 0);
		}
		lastMouseX = 770;
	}
}

void handleActiveMouse(int x, int y){
	handlePassiveMouse(x,y);
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

void playerFire(int button, int state, int x, int y){
	
}

void createTank(int numTanks){
	
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



void createObstacle(float x, float z, float r){
	
}

void drawHealthBars(){
	
	

}

