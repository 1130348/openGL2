#include "Tank.h"
#include "MathForms.h"
#include "Globals.h"



#define LARGURA_BASE        3
#define COMPRIMENTO_BASE    5
#define ALTURA_BASE         0.7

#define LARGURA_TORRE       2
#define COMPRIMENTO_TORRE   2
#define ALTURA_TORRE        0.5

#define COMPRIMENTO_CANHAO  5
#define RAIO_CANHAO         0.2
#define ShootDelay			2000

GLfloat vertices[][3] = { { -0.5,-0.5,-0.5 },
{ 0.5,-0.5,-0.5 },
{ 0.5,0.5,-0.5 },
{ -0.5,0.5,-0.5 },
{ -0.5,-0.5,0.5 },
{ 0.5,-0.5,0.5 },
{ 0.5,0.5,0.5 },
{ -0.5,0.5,0.5 } };

GLfloat cores[][3] = { { 0.0,1.0,1.0 },
{ 1.0,0.0,0.0 },
{ 1.0,1.0,0.0 },
{ 0.0,1.0,0.0 },
{ 1.0,0.0,1.0 },
{ 0.0,0.0,1.0 },
{ 1.0,1.0,1.0 } };


Tank::Tank(float positionX, float positionZ, float initialRotation) {
	this->NextShootTime = 0;
	this->rel = true;
	this->reload = 0;
	this->speed = 0.0f;
	this->posX = positionX;
	this->posZ = positionZ;
	this->rotation = initialRotation;
	this->rotationSpeed = 0.0f;
	this->width = 0.5f;
	this->height = 0.5f;
	this->depth = 0.7f;
	this->turretRotation = 0.0f;
	this->turretRotationSpeed = 0.0f;
	this->health = 60;
	this->reloadTime = 20;
	this->reloadCounter = this->reloadTime;
	this->canSeePlayer = false;
	this->viewCamp = 100.0f;
	this->viewCounter = 0;
	this->isAimed = false;
	this->destinX = positionX;
	this->destinZ = positionZ;
	this->atDestination = true;
	this->recoilStrength = 0.7f;
	this->recoilDistance = 0.0f;
	this->curRecoilForce = 0.0f;
	this->lastViewX = positionX;
	this->lastViewZ = positionZ;
	this->boostSpeed = 0;
	this->boostPower = 0.02f;
	this->shieldStrength = 0;
	this->shieldOpacity = 0.0f;
}

void Tank::accelerate(bool directionIsForward) {
	if (directionIsForward) {
		this->speed += 0.02;
	}
	else {
		this->speed -= 0.02;
	}
}

void Tank::boost() {
	this->boostSpeed += this->boostPower;
}

void Tank::activateShield() {
	if (this->hasShieldLeft()) {
		this->shieldOpacity = 1.0f;
		this->shieldStrength -= 1;
	}
}

bool Tank::hasShieldLeft() {
	return this->shieldStrength > 0;
}

void Tank::rotate(bool directionIsPositive) {
	this->rotationSpeed *= 0.01f;
	if (directionIsPositive) {
		this->rotationSpeed += 3.5;
	}
	else {
		this->rotationSpeed -= 3.5;
	}
	if (this->boostSpeed > 0.02f) {
		this->rotationSpeed /= (this->boostSpeed)*100.0f;
	}
}

void Tank::rotateTurret(float amount) {
	this->turretRotationSpeed += amount;
	if (this->turretRotation > 180) {
		this->turretRotation -= 360;
	}
	else if (this->turretRotation < -180) {
		this->turretRotation += 360;
	}
}

bool Tank::fire() {
	if (NextShootTime < glutGet(GLUT_ELAPSED_TIME))
	{

		if (this->reloadCounter <= 0) {
			float angle = this->rotation + this->turretRotation;
			bullets.push_back(new Bullet(this->posX - 1.0f*sin(angle * PI / 180), 1.0f, (this->posZ - 1.0f *cos(angle * PI / 180)), this->speedX, this->speedZ, angle));
			this->reloadCounter = this->reloadTime;
			this->curRecoilForce += this->recoilStrength;
			reload++;
		}
		if (reload<5) {
			NextShootTime = glutGet(GLUT_ELAPSED_TIME) + ShootDelay;
			rel = true;
		}
		else {
			NextShootTime = glutGet(GLUT_ELAPSED_TIME) + ShootDelay + 2000;
			rel = false;
			reload = 0;
		}
		return true;

	}
	return false;
}

bool Tank::isHitBy(Bullet* theBullet) {
	float x = theBullet->givePosX() - this->givePosX();
	float z = theBullet->givePosZ() - this->givePosZ();
	if (this->hasShieldLeft()) {
		return (x*x) + (z*z) < (this->width)*(this->width);
	}
	else {
		return (x*x) + (z*z) < (this->width * 2)*(this->width * 2);
	}
}

void Tank::move() {
	this->speedX = -(this->speed + this->boostSpeed) * sin(this->rotation * PI / 180);
	this->speedZ = -(this->speed + this->boostSpeed) * cos(this->rotation * PI / 180);

	if (this->canMoveTo(this->posX + this->speedX, this->posZ)) {
		this->posX += this->speedX;
	}
	else {
		this->speedX *= 0.5f;
	}
	if (this->canMoveTo(this->posX, this->posZ + this->speedZ)) {
		this->posZ += this->speedZ;
	}
	else {
		this->speedZ *= 0.5f;
	}


	this->rotation += this->rotationSpeed;
	this->turretRotation -= this->rotationSpeed;
	this->turretRotation += this->turretRotationSpeed;
	this->turretRotationSpeed *= 0.5f;
	this->rotationSpeed *= 0.5f;
	this->speed *= 0.8f;
	this->boostSpeed *= 0.8f;
	if (this->rotation > 360.0f) {
		this->rotation -= 360.0f;
	}
	else if (this->rotation < -360.0f) {
		this->rotation += 360.0f;
	}
	if (this->turretRotation > 360.0f) {
		this->turretRotation -= 360.0f;
	}
	else if (this->turretRotation < -360.0f) {
		this->turretRotation += 360.0f;
	}
	this->reloadCounter -= 1;

	this->recoilDistance += this->curRecoilForce;
	this->curRecoilForce *= 0.5f;
	this->recoilDistance *= 0.8f;
	this->shieldOpacity *= 0.95f;
}

void Tank::runTanksBots() {
	float dist = distanceBetween(this->givePosX(), this->givePosZ(), playerTank->givePosX(), playerTank->givePosZ());

	this->viewCounter -= 1;

	if (dist < this->viewCamp) {
		this->viewCounter = 10;
		this->canSeePlayer = true;
	}
	else {
		if (this->viewCounter < 0) {
			this->canSeePlayer = false;
			this->destinX = this->lastViewX;
			this->destinZ = this->lastViewZ;

		}
	}

	if (this->canSeePlayer) {
		float bulletCyclesToTarget = dist / bulletSpeed;
		float pX = playerTank->givePosX() - bulletCyclesToTarget*playerTank->speed*sin(playerTank->rotation*PI / 180) - bulletCyclesToTarget*this->speedX;
		float pZ = playerTank->givePosZ() - bulletCyclesToTarget*playerTank->speed*cos(playerTank->rotation*PI / 180) - bulletCyclesToTarget*this->speedZ;
		this->turnTurretToward(this->angleTo(pX, pZ));
		if (this->isAimed) {
			this->fire();
		}
		this->lastViewX = playerTank->givePosX();
		this->lastViewZ = playerTank->givePosZ();
	}
	else {
	}

	if (distanceBetween(this->posX, this->posZ, this->destinX, this->destinZ) > 5.0f) {
		this->atDestination = false;
	}
	else {
		this->atDestination = true;
	}


	if (!(rand() % 30) && this->canSeePlayer) {
		this->destinX += rand() % 20 - 10;
		this->destinZ += rand() % 20 - 10;
	}

	if (this->atDestination) {
		this->rotate(true);
		this->accelerate(true);
	}
	else {
		this->turnToward(this->angleTo(this->destinX, this->destinZ));
		this->accelerate(true);
	}
}

float Tank::angleTo(float x, float z) {
	float deltaX = x - this->posX;

	float deltaZ = z - this->posZ;
	if (deltaX >= 0 && deltaZ >= 0) {
		return -180 + ((atan(deltaX / deltaZ)) * 180 / PI);
	}
	else if (deltaX >= 0 && deltaZ < 0) {
		return (atan(deltaX / deltaZ) * 180 / PI);
	}
	else if (deltaX < 0 && deltaZ >= 0) {
		return 180 + ((atan(deltaX / deltaZ)) * 180 / PI);
	}
	else /* (deltaX < 0 && deltaZ < 0) */ {
		return ((atan(deltaX / deltaZ)) * 180 / PI);
	}
	return 0;
}

void Tank::turnTurretToward(float newAngle) {
	this->isAimed = false;
	float curAngle = this->rotation + this->turretRotation;
	if (curAngle > 180) {
		curAngle -= 360;
	}
	else if (curAngle < -180) {
		curAngle += 360;
	}
	if (curAngle > 90 && newAngle < -90) {
		newAngle += 360;
	}
	else if (curAngle < -90 && newAngle > 90) {
		newAngle -= 360;
	}
	if (curAngle < newAngle) {
		if (newAngle - curAngle >= 1.0f) {
			this->rotateTurret(1.0f);
		}
		else {
			this->rotateTurret(newAngle - curAngle);
			this->isAimed = true;
		}
	}
	else if (curAngle > newAngle) {
		if (newAngle - curAngle <= -1.0f) {
			this->rotateTurret(-1.0f);
		}
		else {
			this->rotateTurret(newAngle - curAngle);
			this->isAimed = true;
		}
	}
}

void Tank::turnToward(float newAngle) {
	float curAngle = this->rotation;
	if (curAngle > 180) {
		curAngle -= 360;
	}
	else if (curAngle < -180) {
		curAngle += 360;
	}
	if (curAngle > 90 && newAngle < -90) {
		newAngle += 360;
	}
	else if (curAngle < -90 && newAngle > 90) {
		newAngle -= 360;
	}
	if (curAngle < newAngle) {
		if (newAngle - curAngle >= 3.5f) {
			this->rotate(true);
		}
	}
	else if (curAngle > newAngle) {
		if (newAngle - curAngle <= -3.5f) {
			this->rotate(false);
		}
	}
}

void Tank::damage(int amount) {
	this->health -= amount;
}

int Tank::centerTurret() {
	if (this->turretRotation > 1.0f) {
		this->rotateTurret(-1.0f);
		return -1;
	}
	else if (this->turretRotation < 1.0f) {
		this->rotateTurret(1.0f);
		return 1;
	}
	return 0;
}

bool Tank::isDead() {
	return this->health <= 0;
}

float Tank::givePosX() {
	return this->posX;
}

float Tank::givePosZ() {
	return this->posZ;
}

float Tank::giveSpeedX() {
	return this->speedX;
}

float Tank::giveSpeedZ() {
	return this->speedZ;
}

float Tank::giveRotation() {
	return this->rotation;
}

float Tank::giveRotationSpeed() {
	return this->rotationSpeed;
}

float Tank::giveTurretRotation() {
	return this->turretRotation;
}

float Tank::giveTurretRotationSpeed() {
	return this->turretRotationSpeed;
}

float Tank::giveHealth() {
	return this->health;
}

void Tank::setHealth(int newHealth) {
	this->health = newHealth;
}

void Tank::setShieldStrength(int newStrength) {
	this->shieldStrength = newStrength;
}

int Tank::giveShieldStrength() {
	return this->shieldStrength;
}

bool Tank::canMoveTo(float newX, float newZ) {
	if (newX > mapLimit - 1.0f || newX < -mapLimit + 1.0f || newZ > mapLimit - 1.0f || newZ < -mapLimit + 1.0f) {
		return false;
	}
	for (unsigned int i = 0; i < tanks.size(); i++) {
		if (this != tanks[i] && distanceBetween(newX, newZ, tanks[i]->posX, tanks[i]->posZ) < 4.5f) {
			return false;
		}
	}
	if (this != playerTank && distanceBetween(newX, newZ, playerTank->posX, playerTank->posZ) < 4.5f) {
		return false;
	}
	return true;
}



void desenhaPoligono(GLfloat a[], GLfloat b[], GLfloat c[], GLfloat  d[], GLfloat cor[])
{

	glBegin(GL_POLYGON);
	glColor3fv(cor);
	glTexCoord2f(0, 0);
	glVertex3fv(a);
	glTexCoord2f(1, 0);
	glVertex3fv(b);
	glTexCoord2f(1, 1);
	glVertex3fv(c);
	glTexCoord2f(0, 1);
	glVertex3fv(d);
	glEnd();
}

void desenhaPCano(GLfloat a[], GLfloat b[], GLfloat c[], GLfloat  d[], GLfloat cor[])
{

	glBegin(GL_POLYGON);
	glColor3fv(cor);
	glTexCoord2f(0, 0);
	glVertex3fv(a);
	glTexCoord2f(0.1, 0);
	glVertex3fv(b);
	glTexCoord2f(0.1, 0.5);
	glVertex3fv(c);
	glTexCoord2f(0, 0.5);
	glVertex3fv(d);
	glEnd();
}


void desenhaCubo()
{

	desenhaPoligono(vertices[1], vertices[0], vertices[3], vertices[2], cores[0]);
	desenhaPoligono(vertices[2], vertices[3], vertices[7], vertices[6], cores[1]);
	desenhaPoligono(vertices[3], vertices[0], vertices[4], vertices[7], cores[2]);
	desenhaPoligono(vertices[6], vertices[5], vertices[1], vertices[2], cores[3]);
	desenhaPoligono(vertices[4], vertices[5], vertices[6], vertices[7], cores[4]);
	desenhaPoligono(vertices[5], vertices[4], vertices[0], vertices[1], cores[5]);
}

void desenhaCano()
{

	desenhaPCano(vertices[1], vertices[0], vertices[3], vertices[2], cores[0]);
	desenhaPCano(vertices[2], vertices[3], vertices[7], vertices[6], cores[1]);
	desenhaPCano(vertices[3], vertices[0], vertices[4], vertices[7], cores[2]);
	desenhaPCano(vertices[6], vertices[5], vertices[1], vertices[2], cores[3]);
	desenhaPCano(vertices[4], vertices[5], vertices[6], vertices[7], cores[4]);
	desenhaPCano(vertices[5], vertices[4], vertices[0], vertices[1], cores[5]);
}


void Tank::desenhaTanque()
{

	glPushMatrix(); {
		glTranslated(0, 0, ALTURA_BASE*0.5);
		glScalef(LARGURA_BASE, COMPRIMENTO_BASE, ALTURA_BASE);
		desenhaCubo();
	}glPopMatrix();
	glPushMatrix(); {
		glTranslated(0, 0, 1);
		glRotatef(this->turretRotation, 0.0f, 0.0f, 1.0f);
		glScalef(LARGURA_TORRE, COMPRIMENTO_TORRE, ALTURA_TORRE);
		desenhaCubo();

		glTranslated(0, COMPRIMENTO_TORRE - 1, 0);

		glTranslated(0, -0.2, 0);
		glScalef(0.2, 2.5, 0.3);
		desenhaCano();

	}glPopMatrix();
}

void Tank::buildTank(GLuint textID) {

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textID);

	glPushMatrix(); {
		glTranslatef(this->posX, 0.0f, this->posZ);
		glRotatef(this->rotation, 0.0f, 1.0f, 0.0f);
		glRotatef(90, 1.0f, 0.0f, 0.0f);
		glRotatef(180, 0.0f, 1.0f, 0.0f);
		glRotatef(180, 0.0f, 0.0f, 1.0f);

		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);
		glEnable(GL_DEPTH_TEST);
		desenhaTanque();
	}glPopMatrix();

}

Tank::~Tank() {
}