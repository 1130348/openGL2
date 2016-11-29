

#include "Definitions.h"
#include "Tank.h"

extern bool keyDown[256];
extern bool leftMouseDown;

extern bool isFullscreen;
extern float bulletSpeed;
extern float bulletSpeedOriginal;
extern int playerHealth;

extern float screenShakeMagnitude;
extern float zoomMagnitude;
extern int screenWidth, screenHeight;
extern int numTanks;
extern int numObstacles;
extern float radarVisionMagnitude;
extern bool radarVisionActivated;
extern bool invincibility;

extern bool slowMotionActivated;
extern float slowMotionMagnitude;
extern int slowMotionCounter;
extern float slowMotionDuration;

extern int mapSize;
extern int mapLimit;

extern int lastMouseX;
extern float lagDistance;

extern Tank* playerTank;
extern std::vector<Tank*> tanks;
extern std::vector<Bullet*> bullets;

extern float fogColour[];
extern float originalfogColour[];
