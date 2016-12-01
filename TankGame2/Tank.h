#include "Definitions.h"
#include "Bullet.h"

#ifndef _TANK_H
#define _TANK_H

class Tank {
	public:
		Tank(float positionX, float positionZ, float initialRotation);
		~Tank();
		void accelerate(bool directionIsForward);
		void rotate(bool directionIsPositive);
		void rotateTurret(float amount);
		void move();
		void buildTank(GLuint textID);
		void runTanksBots();
		bool isHitBy(Bullet* theBullet);
		
		float givePosX();
		float givePosZ();
		float giveSpeedX();
		float giveSpeedZ();
		float giveRotation();
		float giveRotationSpeed();
		float giveTurretRotation();
		float giveTurretRotationSpeed();
		float giveHealth();
		void desenhaTanque();
		void damage(int amount);
		bool isDead();
		bool fire();
		float angleTo(float x, float z);
		void turnToward(float newAngle);
		void setHealth(int newHealth);
		int centerTurret();
		bool canMoveTo(float newX, float newZ);
		void boost();
		void activateShield();
		bool hasShieldLeft();
		int giveShieldStrength();
		void setShieldStrength(int newStrength);
		void turnTurretToward(float newAngle);
		
	private:
		float viewCamp;
		int viewCounter;
		float lastViewX, lastViewZ;
		bool canSeePlayer;
		int nBullets = 5;
		
		float speed;
		float posX, posZ;
		float speedX, speedZ;
		float rotation, rotationSpeed;
		float width, height, depth;
		float turretRotation, turretRotationSpeed;
		int health;
		int reloadTime, reloadCounter;
		bool isAimed;
		float destinX, destinZ;
		bool atDestination;
		float boostSpeed, boostPower;
		float recoilStrength, recoilDistance, curRecoilForce;
		int shieldStrength;
		float shieldOpacity;

	
};

#endif