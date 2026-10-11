#include "Enemy.h"
#include "Globals.h"
#include <cmath>

void enemyChasing(glm::vec3 position, glm::vec3& enemyPosition, float& enemyAngle)
{
    // Direzione: enemy -> target
    float dx = position.x - enemyPosition.x;
    float dy = position.y - enemyPosition.y;
    float dz = position.z - enemyPosition.z;

    float length = std::sqrt(dx * dx + dy * dy + dz * dz);

    if (length == 0.0f)
        return;

    // Normalizzazione
    dx /= length;
    dy /= length;
    dz /= length;

    // Movimento = direzione * velocita * deltaTime
    enemyPosition.x += dx * enemySpeed * deltaTime;
    enemyPosition.y += dy * enemySpeed * deltaTime;
    enemyPosition.z += dz * enemySpeed * deltaTime;
    
    enemyAngle = glm::degrees(std::atan2(dx, dz));
}
