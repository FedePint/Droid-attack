#include "Globals.h"

// Audio engine
irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();

// Camera & Lighting
Camera camera(glm::vec3(0.0f, 85.0f, -35.0f));
glm::vec3 lightPos(0.0f, 66.0f, -65.0f);

// Screen resolution
int SCR_WIDTH = 0;
int SCR_HEIGHT = 0;

// Game state
int currentState = 0;
unsigned int gameOverMenuTexture = 0;

// Direction & Mouse tracking
glm::vec3 dir = glm::vec3(0.0f, 1.0f, 0.0f);
float angle = 90.0f;
glm::vec3 mousePos(0.0f, 0.0f, 0.0f);
glm::vec3 mouseDir(0.0f, 0.0f, 0.0f);
float lastX = 0.0f;
float lastY = 0.0f;
bool firstMouse = true;

// Bullets
std::vector<Bullet> bullets;
std::vector<Bullet> Enemybullets;

// Character glyphs for FreeType
std::map<char, Character> Characters;

// Timing & Movement
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float moveSpeed = 30.0f;
float speed = 45.0f;

// Shooting flags & Upgrades
bool canShootUp = true;
bool canShootDown = true;
bool canShootLeft = true;
bool canShootRight = true;
bool PowerUp = false;
bool SpeedUp = false;

// Player properties
glm::vec3 roomSize = glm::vec3(36.8161f, 12.1611f, 36.8602f);
glm::vec3 robotSize = glm::vec3(4.0f, 4.0f, 4.0f);
glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
float robotAngle = 270.0f;
int robotLifePoints = 15;

// Enemy single properties
glm::vec3 enemyPosition = glm::vec3(0.0f, 0.0f, -50.0f);
float enemySpeed = 10.0f;
float enemyAngle = 270.0f;
bool enemyLife = true;

// Multiple enemies
glm::vec3 enemyPositions[3] = {
    glm::vec3(36.8161f - 8.0f, 0.0f, -36.8602f + 8.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(36.8161f - 8.0f, 0.0f, 36.8602f - 8.0f)
};
float enemyAngles[3] = { 270.0f, 270.0f, 270.0f };
bool enemyLives[3] = { true, true, true };

// Boss
glm::vec3 BossPosition = glm::vec3(0.0f, 0.0f, 36.8602f - 36.8602f / 2.0f);
float BossSpeed = 10.0f;
float BossAngle = 270.0f;
int BossLifePoints = 5;

// Cannons
glm::vec3 cannonPositions[4] = {
    glm::vec3(-36.8161f + 8.0f, 0.0f, -36.8602f + 8.0f),
    glm::vec3(36.8161f - 8.0f, 0.0f, -36.8602f + 8.0f),
    glm::vec3(-36.8161f + 8.0f, 0.0f, 36.8602f - 8.0f),
    glm::vec3(36.8161f - 8.0f, 0.0f, 36.8602f - 8.0f)
};
glm::vec3 cannonDirections[4] = {
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f)
};

float zStart = 36.8602f - 4.0f * 36.8602f / 8.0f;
float spacing = (2.0f * 36.8602f / 8.0f);

glm::vec3 cannonPositionsUP[6] = {
    glm::vec3(-36.8161f + 4.0f, 0.0f, 36.8602f - 4.0f * 36.8602f / 8.0f),
    glm::vec3( 36.8161f - 4.0f, 0.0f, (36.8602f - 4.0f * 36.8602f / 8.0f) - (2.0f * 36.8602f / 8.0f)),
    glm::vec3(-36.8161f + 4.0f, 0.0f, (36.8602f - 4.0f * 36.8602f / 8.0f) - 2.0f * (2.0f * 36.8602f / 8.0f)),
    glm::vec3( 36.8161f - 4.0f, 0.0f, (36.8602f - 4.0f * 36.8602f / 8.0f) - 3.0f * (2.0f * 36.8602f / 8.0f)),
    glm::vec3(-36.8161f + 4.0f, 0.0f, (36.8602f - 4.0f * 36.8602f / 8.0f) - 4.0f * (2.0f * 36.8602f / 8.0f)),
    glm::vec3( 36.8161f - 4.0f, 0.0f, (36.8602f - 4.0f * 36.8602f / 8.0f) - 5.0f * (2.0f * 36.8602f / 8.0f))
};

glm::vec3 cannonDirectionsUP[6] = {
    glm::vec3(1.0f, 0.0f, 0.0f),
    glm::vec3(-1.0f, 0.0f, 0.0f),
    glm::vec3(1.0f, 0.0f, 0.0f),
    glm::vec3(-1.0f, 0.0f, 0.0f),
    glm::vec3(1.0f, 0.0f, 0.0f),
    glm::vec3(-1.0f, 0.0f, 0.0f)
};

int cannonLives[4] = { 2, 2, 2, 2 };
int cannonLivesUP[6] = { 2, 2, 2, 2, 2, 2 };

float cannonAngle = 270.0f;
int cannonLifePoints = 3;

int enemykilled = 0;
int bulletsShot = 0;

float vertices[] = {
    // posizioni
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f
};

void ResetGame() {
    // --- NEMICI NORMALI ---
    enemyPositions[0] = glm::vec3(roomSize.x - 8.0f, 0.0f, -roomSize.z + 8.0f);
    enemyPositions[1] = glm::vec3(0.0f, 0.0f, 0.0f);
    enemyPositions[2] = glm::vec3(roomSize.x - 8.0f, 0.0f, roomSize.z - 8.0f);

    for (int i = 0; i < 3; i++) {
        enemyAngles[i] = 270.0f;
        enemyLives[i] = true;
    }

    // --- BOSS ---
    BossPosition = glm::vec3(0.0f, 0.0f, roomSize.z - roomSize.z / 2.0f);
    BossSpeed = 10.0f;
    BossAngle = 270.0f;
    BossLifePoints = 5;

    // --- CANNONI BASE ---
    cannonPositions[0] = glm::vec3(-roomSize.x + 8.0f, 0.0f, -roomSize.z + 8.0f);
    cannonPositions[1] = glm::vec3(roomSize.x - 8.0f, 0.0f, -roomSize.z + 8.0f);
    cannonPositions[2] = glm::vec3(-roomSize.x + 8.0f, 0.0f, roomSize.z - 8.0f);
    cannonPositions[3] = glm::vec3(roomSize.x - 8.0f, 0.0f, roomSize.z - 8.0f);

    for (int i = 0; i < 4; i++) {
        cannonDirections[i] = glm::vec3(0.0f, 0.0f, 0.0f);
        cannonLives[i] = 2;
    }

    // --- CANNONI "UP" ---
    zStart = roomSize.z - 4.0f * roomSize.z / 8.0f;
    spacing = (2.0f * roomSize.z / 8.0f);

    for (int i = 0; i < 6; i++) {
        cannonLivesUP[i] = 2;
        if (i % 2 == 0) {
            cannonPositionsUP[i] = glm::vec3(-roomSize.x + 4.0f, 0.0f, zStart - (i * spacing));
            cannonDirectionsUP[i] = glm::vec3(1.0f, 0.0f, 0.0f);
        } else {
            cannonPositionsUP[i] = glm::vec3(roomSize.x - 4.0f, 0.0f, zStart - (i * spacing));
            cannonDirectionsUP[i] = glm::vec3(-1.0f, 0.0f, 0.0f);
        }
    }

    // --- STATISTICHE GENERALI E CANNONI ---
    cannonAngle = 270.0f;
    cannonLifePoints = 3;
    PowerUp = false;
    SpeedUp = false;
    speed = 45.0f;
}
