#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "GameResources.h"

// Renders the interactive main menu
void RenderMainMenu(const GameResources& res, GLFWwindow* window);

// Renders the game over menu with highscores and stats
void RenderGameOver(const GameResources& res, GLFWwindow* window);
