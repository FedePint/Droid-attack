#include "Input.h"
#include "Globals.h"
#include "Player.h"
#include "Physics.h"
#include <iostream>

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        moveUP(window, true, position);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        moveDOWN(window, true, position);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        moveLEFT(window, true, position);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        moveRIGHT(window, true, position);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && canShootUp)
    {
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE)
    {
        canShootUp = true;
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && canShootDown)
    {
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE)
    {
        canShootDown = true;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && canShootLeft)
    {
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE)
    {
        canShootLeft = true;
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && canShootRight)
    {
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE)
    {
        canShootRight = true;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        if (currentState == 0)
        {
            double btnMinXRatio = 0.40;
            double btnMaxXRatio = 0.60;

            double startMinYRatio = 0.68;
            double startMaxYRatio = 0.79;

            double exitMinYRatio = 0.83;
            double exitMaxYRatio = 0.94;

            double startMinX = SCR_WIDTH * btnMinXRatio;
            double startMaxX = SCR_WIDTH * btnMaxXRatio;
            double startMinY = SCR_HEIGHT * startMinYRatio;
            double startMaxY = SCR_HEIGHT * startMaxYRatio;

            double exitMinX = SCR_WIDTH * btnMinXRatio;
            double exitMaxX = SCR_WIDTH * btnMaxXRatio;
            double exitMinY = SCR_HEIGHT * exitMinYRatio;
            double exitMaxY = SCR_HEIGHT * exitMaxYRatio;

            // START button
            if (mouseX >= startMinX && mouseX <= startMaxX && mouseY >= startMinY && mouseY <= startMaxY) {
                currentState = 1;
            }
            // EXIT button
            else if (mouseX >= exitMinX && mouseX <= exitMaxX && mouseY >= exitMinY && mouseY <= exitMaxY) {
                glfwSetWindowShouldClose(window, true);
            }
        }
        else if (currentState == -1)
        {
            double menuBtnMinX = SCR_WIDTH * (320.0 / 800.0);
            double menuBtnMaxX = SCR_WIDTH * (480.0 / 800.0);
            double menuBtnMinY = SCR_HEIGHT * (520.0 / 600.0);
            double menuBtnMaxY = SCR_HEIGHT * (570.0 / 600.0);

            if (mouseX >= menuBtnMinX && mouseX <= menuBtnMaxX &&
                mouseY >= menuBtnMinY && mouseY <= menuBtnMaxY)
            {
                currentState = 0;
                std::cout << "Torno al Menu Principale" << std::endl;
            }
        }
        else if (currentState == 1)
        {
            if (PowerUp)
            {
                bulletsShot = bulletsShot + 3;
                for (auto& b : bullets) {
                    if (b.active) {
                        canShootUp = false;
                    }
                }

                if (canShootUp)
                {
                    bullets[0].active = true;
                    bullets[0].direction = mouseDir;

                    bullets[1].active = true;
                    bullets[1].direction = rotateY(mouseDir, 22.0f);

                    bullets[3].active = true;
                    bullets[3].direction = rotateY(mouseDir, -22.0f);
                    if (engine) engine->play2D("resources/media/laser.wav");
                }
            }
            else
            {
                bulletsShot++;
                for (auto& b : bullets) {
                    if (!b.active) {
                        shootUp(window, b);
                        break;
                    }
                }
            }
        }
    }
}
