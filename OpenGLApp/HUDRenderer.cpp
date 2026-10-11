#include "HUDRenderer.h"
#include "Globals.h"
#include "RenderUtils.h"
#include <glm/gtc/matrix_transform.hpp>
#include <string>

void RenderHUD(const GameResources& res)
{
    glm::mat4 projectionText = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
    res.textShader->use();
    res.textShader->setMat4("projection", projectionText);

    std::string hpString = "Hp:";
    for (int i = 0; i < robotLifePoints; i++) {
        hpString += "|";
    }

    glm::vec3 hpColor;
    if (robotLifePoints > 10) {
        hpColor = glm::vec3(0.0f, 1.0f, 0.0f);
    }
    else if (robotLifePoints > 5) {
        hpColor = glm::vec3(1.0f, 1.0f, 0.0f);
    }
    else {
        hpColor = glm::vec3(1.0f, 0.0f, 0.0f);
    }

    RenderText(*res.textShader, hpString, 10.0f, static_cast<float>(SCR_HEIGHT) / 15.0f, 1.0f, hpColor);

    float powerUpPosY = static_cast<float>(SCR_HEIGHT) / 15.0f;
    float powerUpPosX = static_cast<float>(SCR_WIDTH) - static_cast<float>(SCR_WIDTH) / 8.0f;

    if (SpeedUp) {
        RenderText(*res.textShader, "SPEED BOOST", powerUpPosX, powerUpPosY, 1.0f, glm::vec3(0.2f, 0.8f, 1.0f));
        powerUpPosY += static_cast<float>(SCR_HEIGHT) / 15.0f;
    }

    if (PowerUp) {
        RenderText(*res.textShader, " MULTI-SHOT", powerUpPosX, powerUpPosY, 1.0f, glm::vec3(1.0f, 0.6f, 0.0f));
    }
}
