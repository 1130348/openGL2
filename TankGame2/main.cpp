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
	glutKeyboardFunc(Key);
	glutKeyboardUpFunc(KeyUp);
	glutMotionFunc(handleActiveMouse);
	glutPassiveMotionFunc(handlePassiveMouse);
	glutReshapeFunc(Reshape);
	glutIgnoreKeyRepeat(true);
	if (isFullscreen){
		glutSetCursor(GLUT_CURSOR_NONE);
	}
	glutMouseFunc(playerFire);
	glutIgnoreKeyRepeat(1);
	
	playerTank = new Tank(0.0f, 0.0f, 0.0f);
	playerTank->setHealth(playerHealth);
	playerTank->setShieldStrength(10);
	srand(time(0));
	//glutTimerFunc(0, timer, 0);
	glutMainLoop();
	return 0;
}

//Initializes 3D rendering
void initRendering() {
	
	glClearColor(0.0, 0.0, 0.0, 0.0);
	//glClearColor(fogColour[0], fogColour[1], fogColour[2], fogColour[3]);

	/*glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);*/
	// set the fog attributes
	/*glFogf(GL_FOG_START,  1.0f);
	glFogf(GL_FOG_END,    200.0f);
	glFogfv(GL_FOG_COLOR,  fogColour);
	glFogi(GL_FOG_MODE,   GL_EXP);
	glFogf(GL_FOG_DENSITY, 0.1f);
	
	// enable the fog
	glEnable(GL_FOG);*/
	
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	/*glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING); //Enable lighting
	glEnable(GL_LIGHT0); //Enable light #0
	glEnable(GL_LIGHT1); //Enable light #1
	glEnable(GL_NORMALIZE); //Automatically normalize normals
	glShadeModel(GL_SMOOTH); //Enable smooth shading*/
	
	
}

void timer(int n) {

	glutTimerFunc(n, timer, n);

	glutPostRedisplay();


}

//Draws the 3D scene
void drawScene() {

	//glFogfv(GL_FOG_COLOR, fogColour);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	
	glPushMatrix();
		
	float x = 0.1f * (rand() % 10); //Move a origem do referencial do ecra para um lugar aleatório
	float y = 0.1f * (rand() % 10);
	float z = 0.1f * (rand() % 10);


	//makeReferenceCubes(1.5f, 0.1f);

	//makeGrid(mapSize);

	if (TPS) {
		printf("TPS \n");
	
		glRotatef(screenShakeMagnitude, x, y, z);
		glTranslatef(0.0f, -1.5f, -3.0f);
		glRotatef(10, 1.0f, 0.0f, 0.0f);

		glTranslatef(0.0f, -0.0f, -3.0f);
		glRotatef(10, 1.0f, 0.0f, 0.0f);
	}
	else if (FPS) {
		printf("FPS \n");
		glRotatef(screenShakeMagnitude, x, y, z);
		glTranslatef(0.0f, -1.5f, -3.0f);
		glRotatef(10, 1.0f, 0.0f, 0.0f);
	}
	else if(VistaTopo){
		printf("VP \n");
		gluLookAt(0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0);
		glRotatef(screenShakeMagnitude, x, y, z);
	}
	
	if (GRID) {
		makeGrid(mapSize);
	}

	glPushMatrix();

		glTranslatef(playerTank->givePosX(), 1.0f, playerTank->givePosZ());
		glRotatef(playerTank->giveRotation() + playerTank->giveTurretRotation(), 0.0f, 1.0f, 0.0f);
		glColor4f(0.0f, 5.0f, 0.0f, 0.2f);

		if (TARGET) {
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
		}


		glPopMatrix();

		playerTank->drawSelf();


	glPopMatrix();
	
	//glDisable(GL_LIGHTING);
	drawHealthBars();
	//glEnable(GL_LIGHTING);

	glutSwapBuffers();
}

void renderBitmapString(float x, float y, float z, void *font, char *string) {  

	char *c;
	glRasterPos3f(x, y,z);
	for (c=string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
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

/* Callback para interaccao via teclado (carregar na tecla) */
void Key(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		exit(1);
		/* ... accoes sobre outras teclas ... */

	case 'h':
	case 'H':
		imprime_ajuda();
		break;
	case '+':
		
		break;

	case '-':
		
		break;

	case 'w':
	case 'W':
		playerTank->accelerate(true);
		playerTank->move();
		glutPostRedisplay();
		break;
	case 'p':
	case 'P':
		
		FPS = GL_TRUE;
		TPS = GL_FALSE;
		VistaTopo = GL_FALSE;
		glutPostRedisplay();
		break;

	case 't':
	case 'T':

		FPS = GL_FALSE;
		TPS = GL_TRUE;
		VistaTopo = GL_FALSE;
		glutPostRedisplay();
		break;

	case 'v':
	case 'V':

		FPS = GL_FALSE;
		TPS = GL_FALSE;
		VistaTopo = GL_TRUE;
		glutPostRedisplay();
		break;
	case 'g':
	case 'G':
		if (GRID) {
			GRID = GL_FALSE;
		}
		else {
			GRID = GL_TRUE;
		}

		glutPostRedisplay();
		break;
	case 'u':
	case 'U':

		if (TARGET) {
			TARGET = GL_FALSE;
		}
		else {
			TARGET = GL_TRUE;
		}
		glutPostRedisplay();
		break;

	}

	if (DEBUG)
		printf("Carregou na tecla %c\n", key);

}

void SpecialKey(int key, int x, int y)
{
	/* ... accoes sobre outras teclas especiais ...
	GLUT_KEY_F1 ... GLUT_KEY_F12
	GLUT_KEY_UP
	GLUT_KEY_DOWN
	GLUT_KEY_LEFT
	GLUT_KEY_RIGHT
	GLUT_KEY_PAGE_UP
	GLUT_KEY_PAGE_DOWN
	GLUT_KEY_HOME
	GLUT_KEY_END
	GLUT_KEY_INSERT
	*/

	switch (key) {

		/* redesenhar o ecra */
		//glutPostRedisplay();
	case GLUT_KEY_F1:
		
		glutPostRedisplay();
		break;

	}


	if (DEBUG)
		printf("Carregou na tecla especial %d\n", key);
}


/* Callback para interaccao via teclado (largar a tecla) */
void KeyUp(unsigned char key, int x, int y)
{

	if (DEBUG)
		printf("Largou a tecla %c\n", key);
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

void createTank(float x, float y){
	
}

void createObstacle(float x, float z, float r){
	
}

void drawHealthBars(){
	
	

}

