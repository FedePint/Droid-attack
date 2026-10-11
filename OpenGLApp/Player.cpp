#include "Player.h"
#include "Globals.h"

void moveUP(GLFWwindow* window, bool state, glm::vec3& position)
{      
    if ((position.z + robotSize.z) < roomSize.z)
    {
        position.z += moveSpeed * deltaTime;
    }
    angle = 270.0f;
}

void moveDOWN(GLFWwindow* window, bool state, glm::vec3& position)
{
    if ((position.z - robotSize.z) > -roomSize.z)
    {
        position.z -= moveSpeed * deltaTime;
    }
    angle = 90.0f;
}

void moveLEFT(GLFWwindow* window, bool state, glm::vec3& position)
{
    if ((position.x + robotSize.x) < roomSize.x)
    {
        position.x += moveSpeed * deltaTime;
    }
    angle = 0.0f;
}

void moveRIGHT(GLFWwindow* window, bool state, glm::vec3& position)
{
    if ((position.x - robotSize.x) > -roomSize.x)
    {
        position.x -= moveSpeed * deltaTime;
    }
    angle = 180.0f;
}

void shootUp(GLFWwindow* window, Bullet& bullet)
{
    bullet.active = true;
    bullet.direction = mouseDir;
    if (engine) engine->play2D("resources/media/laser.wav");
}

void shootDown(GLFWwindow* window, Bullet& bullet)
{
    bullet.active = true;
    bullet.direction = glm::vec3(0.0f, 0.0f, -1.0f);
    if (engine) engine->play2D("resources/media/laser.wav");
}

void shootLeft(GLFWwindow* window, Bullet& bullet)
{
    bullet.active = true;
    bullet.direction = glm::vec3(1.0f, 0.0f, 0.0f);
    if (engine) engine->play2D("resources/media/laser.wav");
}

void shootRight(GLFWwindow* window, Bullet& bullet)
{
    bullet.active = true;
    bullet.direction = glm::vec3(-1.0f, 0.0f, 0.0f);
    if (engine) engine->play2D("resources/media/laser.wav");
}

void bossShoot(Bullet& bullet, const glm::vec3& Direction)
{
    bullet.active = true;
    bullet.direction = Direction;
    if (engine) engine->play2D("resources/media/laser.wav");
}

void BulletActive(std::vector<Bullet>& bullets)
{
    for (size_t i = 0; i < bullets.size(); i++)
    {
        if (bullets[i].active)
        {
            bullets[i].position += bullets[i].direction * speed * deltaTime;
            if (bullets[i].position.z < roomSize.z && bullets[i].position.z > -roomSize.z &&
                bullets[i].position.x < roomSize.x && bullets[i].position.x > -roomSize.x)
            {
                bullets[i].active = true;
            }
            else
            {
                bullets[i].active = false;
                bullets[i].position = position;
            }
        }
        else
        {
            bullets[i].position = position;
        }
    }
}
