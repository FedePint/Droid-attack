#pragma once

#include <memory>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "shader.h"
#include "model.h"

// Holds all loaded shaders, 3D models, textures, and vertex buffers
struct GameResources {
    // Shaders
    std::unique_ptr<Shader> ourShader;
    std::unique_ptr<Shader> lightCubeShader;
    std::unique_ptr<Shader> roomShader;
    std::unique_ptr<Shader> bulletShader;
    std::unique_ptr<Shader> textShader;
    std::unique_ptr<Shader> enemyShader;
    std::unique_ptr<Shader> cannonShader;
    std::unique_ptr<Shader> radarShader;
    std::unique_ptr<Shader> gemShader;
    std::unique_ptr<Shader> puddleShader;
    std::unique_ptr<Shader> portalShader;
    std::unique_ptr<Shader> menuShader;

    // 3D Models
    std::unique_ptr<Model> ourModel;
    std::unique_ptr<Model> room;
    std::unique_ptr<Model> bulletModel;
    std::unique_ptr<Model> enemyModel;
    std::unique_ptr<Model> bossModel;
    std::unique_ptr<Model> cannonModel;
    std::unique_ptr<Model> gemModel;
    std::unique_ptr<Model> puddleModel;
    std::unique_ptr<Model> portalModel;

    // Vertex arrays & textures
    unsigned int menuVAO = 0;
    unsigned int menuVBO = 0;
    unsigned int menuEBO = 0;
    unsigned int menuTexture = 0;

    unsigned int lightCubeVAO = 0;
    unsigned int lightCubeVBO = 0;

    // FreeType library handles
    FT_Library ft = nullptr;
    FT_Face face = nullptr;

    // Room bounds and exits
    glm::vec3 minRoom = glm::vec3(0.0f);
    glm::vec3 maxRoom = glm::vec3(0.0f);
    glm::vec3 exitRoom1R = glm::vec3(0.0f);
    glm::vec3 exitRoom2UP = glm::vec3(0.0f);

    // Initialize all assets, shaders, textures, and buffers
    bool Init();

    // Release GPU and FreeType resources
    void Cleanup();
};
