#include "RadarRenderer.h"
#include "Globals.h"
#include <glm/gtc/matrix_transform.hpp>

void RenderRadar(const GameResources& res, int roomActive)
{
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    glm::mat4 projRadar = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
    res.radarShader->use();
    res.radarShader->setMat4("projection", projRadar);

    float radarRadius = 85.0f;
    float radarCenterX = SCR_WIDTH - radarRadius - 15.0f;
    float radarCenterY = SCR_HEIGHT - radarRadius - 15.0f;
    float worldMaxLimit = roomSize.x;

    // 1. Radar background (Dark green/grey semi-transparent disc)
    glm::mat4 bgModel = glm::mat4(1.0f);
    bgModel = glm::translate(bgModel, glm::vec3(radarCenterX, radarCenterY, 0.0f));
    bgModel = glm::scale(bgModel, glm::vec3(radarRadius, radarRadius, 1.0f));
    res.radarShader->setMat4("model", bgModel);
    res.radarShader->setVec3("pointColor", glm::vec3(0.1f, 0.3f, 0.1f));
    res.radarShader->setFloat("alpha", 0.6f);
    glBindVertexArray(res.menuVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Lambda to draw individual blips
    auto drawRadarPoint = [&](glm::vec3 worldPos, glm::vec3 color, float pointSize) {
        float rx = radarCenterX - (worldPos.x / worldMaxLimit) * radarRadius;
        float ry = radarCenterY + (worldPos.z / worldMaxLimit) * radarRadius;

        glm::mat4 ptModel = glm::mat4(1.0f);
        ptModel = glm::translate(ptModel, glm::vec3(rx, ry, 0.0f));
        ptModel = glm::scale(ptModel, glm::vec3(pointSize, pointSize, 1.0f));

        res.radarShader->setMat4("model", ptModel);
        res.radarShader->setVec3("pointColor", color);
        res.radarShader->setFloat("alpha", 1.0f);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    };

    // 2. Draw Player (Green dot)
    drawRadarPoint(position, glm::vec3(0.0f, 1.0f, 0.0f), 3.0f);

    // 3. Draw Enemies (Red dots)
    if (roomActive == 4) {
        for (int i = 0; i < 3; i++) {
            if (enemyLives[i]) {
                drawRadarPoint(enemyPositions[i], glm::vec3(1.0f, 0.0f, 0.0f), 2.5f);
            }
        }
    }
    else if (roomActive == 1) {
        for (int i = 0; i < 4; i++) {
            if (cannonLives[i]) {
                drawRadarPoint(cannonPositions[i], glm::vec3(1.0f, 0.0f, 0.0f), 2.5f);
            }
        }
    }
    else if (roomActive == 3) {
        for (int i = 0; i < 6; i++) {
            if (cannonLivesUP[i]) {
                drawRadarPoint(cannonPositionsUP[i], glm::vec3(1.0f, 0.0f, 0.0f), 2.5f);
            }
        }
    }
    else if (roomActive == 2 && BossLifePoints >= 0) {
        // Boss (Purple dot, larger)
        drawRadarPoint(BossPosition, glm::vec3(1.0f, 0.0f, 1.0f), 5.0f);
    }

    glEnable(GL_DEPTH_TEST);
}
