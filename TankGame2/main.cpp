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
GLboolean   AJUDA;
GLboolean paused;

int main(int argc, char** argv) {

	DEBUG = GL_TRUE;
	TPS = GL_TRUE;
	FPS = GL_FALSE;
	VistaTopo = GL_FALSE;
	GRID = GL_TRUE;
	TARGET = GL_TRUE;
	AJUDA = GL_FALSE;
	paused = GL_FALSE;



	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(glutGet(GLUT_SCREEN_WIDTH) - 200, glutGet(GLUT_SCREEN_HEIGHT) - 200);
	glutInitWindowPosition(GLUT_SCREEN_HEIGHT/2, GLUT_SCREEN_WIDTH / 2);
	
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

	glutIgnoreKeyRepeat(true);
	if (isFullscreen) {
		glutSetCursor(GLUT_CURSOR_NONE);
	}
	glutMouseFunc(playerFire);

	glutReshapeFunc(Reshape);
	
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
	//Adiconado Light (FIX PLS)
	// set the fog attributes
	/*glFogf(GL_FOG_START, 1.0f);
	glFogf(GL_FOG_END, 200.0f);
	glFogfv(GL_FOG_COLOR, fogColour);
	glFogi(GL_FOG_MODE, GL_EXP);
	glFogf(GL_FOG_DENSITY, 0.1f);*/

	// enable the fog
	//glEnable(GL_FOG);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_COLOR_MATERIAL);
	//glEnable(GL_LIGHTING); //Enable lighting
	//glEnable(GL_LIGHT0); //Enable light #0
	//glEnable(GL_LIGHT1); //Enable light #1
	//glEnable(GL_NORMALIZE); //Automatically normalize normals
	//glShadeModel(GL_SMOOTH); //Enable smooth shading
							 //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//Wireframe

}

void timer(int n) {

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
		
		}
		if (numTanks <= 0) {
		
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
				tanks[j]->damage(10);
				bullets[i]->flagAsDead();
				bulletNotDead = false;
			}
		}
		if (bulletNotDead && playerTank->isHitBy(bullets[i])) {
			if (playerTank->hasShieldLeft()) {
				playerTank->activateShield();
			}
			else if (!invincibility) {
				playerTank->damage(10);
				screenShakeMagnitude += 1.0f;
			}
			bullets[i]->flagAsDead();
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
	if (keyDown['n']) {
		//rodar torre dir
		/*playerTank->rotateTurret(10);
		playerTank->move();*/
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
		//rodar torre esq
		/*playerTank->rotateTurret(-10);
		playerTank->move();*/
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

	if (keyDown['x'] || leftMouseDown) {
		if (playerTank->fire()) {
			screenShakeMagnitude += 0.1f;
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

	if (keyDown['h']) {
		
		if (AJUDA) {
			AJUDA = GL_FALSE;
			paused = !paused;
			//imprime_ajuda();
		}
		else {
			AJUDA = GL_TRUE;
				
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
		drawBullets();
		//drawPiramids();

		for (unsigned int i = 0; i < tanks.size(); i++) {

			glColor3f(1.0, 0, 1);
			tanks[i]->buildTank();
		}
			glPushMatrix();

			glTranslatef(playerTank->givePosX(), 1.0f, playerTank->givePosZ());
			glRotatef(playerTank->giveRotation() + playerTank->giveTurretRotation(), 0.0f, 1.0f, 0.0f);
			glColor3f(1,0 , 0);

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
		glColor3f(0, 2.0f, 0.3f);
		playerTank->buildTank();

	glPopMatrix();
	
	//glDisable(GL_LIGHTING);
	//drawHealthBars();
	//glEnable(GL_LIGHTING);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (AJUDA) {

		glColor3f(0, 0, 0);

		printtext(520, 450, "Controlos:");
		printtext(520, 400, "h,H - Ajuda ");
		printtext(520, 350, "P   - Vista Primeira Pessoa");
		printtext(520, 300, "T   - Vista De Topo");
		printtext(520, 250, "V   - Vista Terceira Pessoa");
		printtext(520, 200, "G   - Grid ON/OFF");
		printtext(520, 150, "U   - Target ON/OFF");
		printtext(520, 100, "ESC - Sair");
		paused = !paused;

	}

	glColor3f(1, 0, 0);
	printtext(50, 50, "HEALTH: 100%");
	printtext(50, 30, "1/5 RELOAD");
	
	glutSwapBuffers();
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

//void drawPiramids() {
//	glEnable(GL_POINT_SMOOTH);
//	glEnable(GL_LINE_SMOOTH);
//	glEnable(GL_POLYGON_SMOOTH);
//	glEnable(GL_DEPTH_TEST);
//	GLfloat places[][3] = { { 4,1,4 },{ -5,1,1 },{ 9,1,6 },{ 6,1,-2 },{-1,1,4 },{-10,1,7},{-3,1,-5}};
//
//	for (int i = 0; i < 6; i++) {
//		glPushMatrix(); {
//			glScalef(7, 2, 2);
//			
//			glTranslatef( places[i][0], places[i][1],places[i][2]);
//			makePyramid();
//
//		}glPopMatrix();
//	}
//}
//
//void makePyramid()
//{
//	glBegin(GL_TRIANGLES);
//	glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.f, 0.0f);
//	glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
//	glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);
//
//	glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
//	glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
//	glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(0.0f, -1.0f, -1.0f);
//
//	glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
//	glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(0.0f, -1.0f, -1.0f);
//	glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);
//
//
//	glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
//	glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(0.0f, -1.0f, -1.0f);
//	glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);
//
//	glEnd();
//
//}

