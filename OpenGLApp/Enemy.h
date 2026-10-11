#pragma once

#include <glm/glm.hpp>

// Enemy AI: chases the target position and updates orientation angle
void enemyChasing(glm::vec3 position, glm::vec3& enemyPosition, float& enemyAngle);
