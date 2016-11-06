#include "Definitions.h"

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
		void drawSelf();

		float givePosX();
		float givePosZ();
		float giveSpeedX();
		float giveSpeedZ();
		float giveRotation();
		float giveRotationSpeed();
		float giveTurretRotation();
		float giveTurretRotationSpeed();
		float giveHealth();
		void damage(int amount);
		bool isDead();
		void runAI();
		float angleTo(float x, float z);
		void turnTurretToward(float newAngle);
		void turnToward(float newAngle);
		void setHealth(int newHealth);
		int centerTurret();
		bool canMoveTo(float newX, float newZ);
		void boost();
		float giveBoostSpeed();
		
	private:
		float speed;
		float posX, posZ;
		float speedX, speedZ;
		float rotation, rotationSpeed;
		float width, height, depth;
		float turretRotation, turretRotationSpeed;
		int health;
		int reloadTime, reloadCounter;
		bool canSeePlayer;
		float sightRange;
		int sightCounter;
		bool isAimed;
		float destinX, destinZ;
		float lastSightingX, lastSightingZ;
		bool atDestination;
		float boostSpeed, boostPower;
		float recoilStrength, recoilDistance, curRecoilForce;

	
};

#endif