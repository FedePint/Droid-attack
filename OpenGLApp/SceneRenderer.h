#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "GameResources.h"

// Renders the 3D game world: player, rooms, portals, enemies, cannons, boss, bullets, and collisions
void RenderScene(const GameResources& res, int& roomActive, int& roomPrev, const glm::mat4& view, const glm::mat4& projection);
