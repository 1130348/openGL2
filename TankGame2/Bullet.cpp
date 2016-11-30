#include "Bullet.h"
#include "MathForms.h"
#include "Globals.h"


Bullet::Bullet(float positionX, float positionY, float positionZ, float velX, float velZ, float initialRotation) {

	this->originalSpeedX = velX - bulletSpeedOriginal * sin(PI / 180 * (initialRotation));
	this->originalSpeedZ = velZ - bulletSpeedOriginal * cos(PI / 180 * (initialRotation));
	this->speedX = this->originalSpeedX;
	this->speedZ = this->originalSpeedZ;
	this->posX = positionX;
	this->posY = positionY;
	this->posZ = positionZ;
	this->remainingLife = 200;
}

void Bullet::drawBullet() {
	glPushMatrix();
	glTranslatef(this->posX, this->posY, this->posZ);
	glColor3f(0.0f, 0.0f, 0.0f);
	glutSolidSphere(0.10, 10, 10);
	glPopMatrix();
}

void Bullet::move() {
	this->speedX = this->originalSpeedX * (slowMotionDuration - slowMotionMagnitude) / slowMotionDuration;
	this->speedZ = this->originalSpeedZ * (slowMotionDuration - slowMotionMagnitude) / slowMotionDuration;
	this->posX += this->speedX;
	this->posZ += this->speedZ;
	if (!slowMotionMagnitude > 0.0f) {
		this->remainingLife--;
	}
	this->remainingLife -= 1;
}

void Bullet::flagAsDead() {
	this->remainingLife = 0;
}

bool Bullet::isDead() {
	return this->remainingLife <= 0;
}

float Bullet::givePosX() {
	return this->posX;
}
float Bullet::givePosZ() {
	return this->posZ;
}
