#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <irrKlang.h>
#include "camera.h"
#include "GameTypes.h"
#include <vector>
#include <map>

// Audio engine
extern irrklang::ISoundEngine* engine;

// Camera & Lighting
extern Camera camera;
extern glm::vec3 lightPos;

// Screen resolution
extern int SCR_WIDTH;
extern int SCR_HEIGHT;

// Game state
extern int currentState;
extern unsigned int gameOverMenuTexture;

// Direction & Mouse tracking
extern glm::vec3 dir;
extern float angle;
extern glm::vec3 mousePos;
extern glm::vec3 mouseDir;
extern float lastX;
extern float lastY;
extern bool firstMouse;

// Bullets
extern std::vector<Bullet> bullets;
extern std::vector<Bullet> Enemybullets;

// Character glyphs for FreeType
extern std::map<char, Character> Characters;

// Timing & Movement
extern float deltaTime;
extern float lastFrame;
extern float moveSpeed;
extern float speed;

// Shooting flags & Upgrades
extern bool canShootUp;
extern bool canShootDown;
extern bool canShootLeft;
extern bool canShootRight;
extern bool PowerUp;
extern bool SpeedUp;

// Player properties
extern glm::vec3 roomSize;
extern glm::vec3 robotSize;
extern glm::vec3 position;
extern float robotAngle;
extern int robotLifePoints;

// Enemy single properties
extern glm::vec3 enemyPosition;
extern float enemySpeed;
extern float enemyAngle;
extern bool enemyLife;

// Multiple enemies
extern glm::vec3 enemyPositions[3];
extern float enemyAngles[3];
extern bool enemyLives[3];

// Boss
extern glm::vec3 BossPosition;
extern float BossSpeed;
extern float BossAngle;
extern int BossLifePoints;

// Cannons
extern glm::vec3 cannonPositions[4];
extern glm::vec3 cannonDirections[4];
extern float zStart;
extern float spacing;
extern glm::vec3 cannonPositionsUP[6];
extern glm::vec3 cannonDirectionsUP[6];
extern int cannonLives[4];
extern int cannonLivesUP[6];
extern float cannonAngle;
extern int cannonLifePoints;

// Game statistics
extern int enemykilled;
extern int bulletsShot;

// Cube vertex buffer data
extern float vertices[108];

// Reset game state
void ResetGame();
