#include "MenuRenderer.h"
#include "Globals.h"
#include "RenderUtils.h"
#include <glm/gtc/matrix_transform.hpp>
#include <string>

void RenderMainMenu(const GameResources& res, GLFWwindow* window)
{
    enemykilled = 0;
    bulletsShot = 0;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    res.menuShader->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, res.menuTexture);

    glBindVertexArray(res.menuVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void RenderGameOver(const GameResources& res, GLFWwindow* window)
{
    int BM = 0;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    res.menuShader->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gameOverMenuTexture);

    glDisable(GL_DEPTH_TEST);

    glBindVertexArray(res.menuVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glm::mat4 projectionText = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
    res.textShader->use();
    res.textShader->setMat4("projection", projectionText);

    std::string recordText = "Kill: " + std::to_string(enemykilled);
    std::string recordText2 = "Shot: " + std::to_string(bulletsShot);
    if (PowerUp) BM++;
    if (SpeedUp) BM++;
    std::string recordText3 = "Bonus " + std::to_string(BM);

    float textScale = 1.2f;
    glm::vec3 textColor = glm::vec3(0.8f, 0.1f, 0.1f);

    float textPosX = SCR_WIDTH / 2.3f;
    float textPosY = SCR_HEIGHT / 2.25f;

    RenderText(*res.textShader, recordText, textPosX, textPosY, textScale, textColor);
    RenderText(*res.textShader, recordText2, textPosX, textPosY - 50.0f, textScale, textColor);
    RenderText(*res.textShader, recordText3, textPosX, textPosY - 100.0f, textScale, textColor);

    glEnable(GL_DEPTH_TEST);
    glfwSwapBuffers(window);
    glfwPollEvents();

    for (int i = 0; i < 4; i++) {
        bullets[i].active = false;
        bullets[i].position = position;
    }

    ResetGame();
}
