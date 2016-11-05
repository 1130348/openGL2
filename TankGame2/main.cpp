#include <ctime>
#include "main.h"
#include "GlobalDeclarations.h"


int main(int argc, char** argv) {
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
	glutReshapeFunc(handleResize);
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
	glutMainLoop();
	return 0;
}

//Initializes 3D rendering
void initRendering() {
	
	glClearColor(fogColour[0], fogColour[1], fogColour[2], fogColour[3]);	
	
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

void timer(int value) {
}

//Draws the 3D scene
void drawScene() {

	//glFogfv(GL_FOG_COLOR, fogColour);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glPushMatrix();{
		
	float x = 0.1f * (rand() % 10); //Move a origem do referencial do ecra para um lugar aleatório
	float y = 0.1f * (rand() % 10);
	float z = 0.1f * (rand() % 10);

	//vista de topo
	//gluLookAt(0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0);

	//gluLookAt(playerTank->givePosX(), 0, playerTank->givePosZ(), 0, 0, 0, 0, 0, 0);


	glRotatef(screenShakeMagnitude, x, y, z);

	//FPS or TPS
	glTranslatef(0.0f, -1.5f, -3.0f);
	glRotatef(10, 1.0f, 0.0f, 0.0f);
	/*glRotatef(-playerTank->giveRotation(), 0.0f, 1.0f, 0.0f);
	glRotatef(lagDistance, 0.0f, 1.0f, 0.0f);
	glRotatef(-playerTank->giveTurretRotation(), 0.0f, 1.0f, 0.0f);
	glTranslatef(-playerTank->givePosX(), 0.0f, -playerTank->givePosZ());*/

	glPushMatrix(); {
		glTranslatef(playerTank->givePosX(), 1.0f, playerTank->givePosZ());
		glRotatef(playerTank->giveRotation() + playerTank->giveTurretRotation(), 0.0f, 1.0f, 0.0f);
		glColor4f(1.0f, 5.0f, 0.0f, 0.2f);
		float seperation = 3.0f;
	}glPopMatrix();

	playerTank->drawSelf();
	}glPopMatrix();
	
	setOrthographicProjection();
		glPushMatrix();
			glLoadIdentity();
			renderBitmapString(720, 450, 10, GLUT_BITMAP_HELVETICA_18, "Hello World!");
		glPopMatrix();
	resetPerspectiveProjection();
		
	glutSwapBuffers();
}

void renderBitmapString(float x, float y, float z, void *font, char *string) {  
	char *c;
	glRasterPos3f(x, y,z);
	for (c=string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
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

void checkInput(){
	if(keyDown[27]){
		exit(0);
	}
	
}

//Called when a key is pressed
void handleKeypress(unsigned char key, int x, int y) {    //The current mouse coordinates
	keyDown[key] = true;
}

void handleKeyUp(unsigned char key, int x, int y){
	keyDown[key] = false;
}

//Called when the window is resized
void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
	screenWidth = w;
	screenHeight = h;
}

void playerFire(int button, int state, int x, int y){
	
}

void createTank(float x, float y){
	
}

void createObstacle(float x, float z, float r){
	
}

void drawHealthBars(){
	
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

}

