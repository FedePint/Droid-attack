#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "model.h"

// Check spherical / circular 2D (XZ) collision between two centers with given radii
bool hitbox(glm::vec3 centro1, double r1, glm::vec3 centro2, double r2);

// Rotate a 3D vector around the Y axis by degrees
glm::vec3 rotateY(const glm::vec3& v, float degrees);

// Compute bounding box minimum point of a room model
glm::vec3 getRoomMin(const Model& room);

// Compute bounding box maximum point of a room model
glm::vec3 getRoomMax(const Model& room);
