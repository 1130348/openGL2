bool keyDown[256];
bool leftMouseDown = false;

bool isFullscreen = true;
float bulletSpeed = 0.5f;
float bulletSpeedOriginal = 0.5f;
int playerHealth = 150;
float screenShakeMagnitude= 0.0f;
float zoomMagnitude = 0.0f;
int screenWidth = 0, screenHeight = 0;
int numTanks = 5;
int numObstacles = 20;
float radarVisionMagnitude = 0;
bool radarVisionActivated = false;
bool slowMotionActivated = false;
float slowMotionMagnitude = 0;
int slowMotionCounter = 0;
float slowMotionDuration = 20.0f;
int mapSize = 70;
int mapLimit = 50;
bool invincibility = false;

int lastMouseX = 770;
float lagDistance = 0;


Tank* playerTank;
Bullet* bullet;

std::vector<Tank*> tanks;
std::vector<Bullet*>bullets;

float fogColour[] = { 0.5, 0.5, 0.5, 1.0 };
float originalfogColour[] = {1.0f,2.0f,0.79f,0.0f};
