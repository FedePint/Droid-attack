#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "GameTypes.h"

// Player movement
void moveUP(GLFWwindow* window, bool state, glm::vec3& position);
void moveDOWN(GLFWwindow* window, bool state, glm::vec3& position);
void moveLEFT(GLFWwindow* window, bool state, glm::vec3& position);
void moveRIGHT(GLFWwindow* window, bool state, glm::vec3& position);

// Shooting functions
void shootUp(GLFWwindow* window, Bullet& bullet);
void shootDown(GLFWwindow* window, Bullet& bullet);
void shootLeft(GLFWwindow* window, Bullet& bullet);
void shootRight(GLFWwindow* window, Bullet& bullet);
void bossShoot(Bullet& bullet, const glm::vec3& Direction);

// Update active bullets
void BulletActive(std::vector<Bullet>& bullets);
