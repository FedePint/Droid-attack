#include "GameResources.h"
#include "Globals.h"
#include "Physics.h"
#include "stb_image.h"
#include <iostream>

bool GameResources::Init() {
    // 1. Initialize FreeType
    if (FT_Init_FreeType(&ft)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return false;
    }

    std::string font_name = "resources/fonts/Antonio/static/Antonio-Bold.ttf";
    if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return false;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
            continue;
        }

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
            face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }

    // 2. Light cube vertex array
    glGenVertexArrays(1, &lightCubeVAO);
    glGenBuffers(1, &lightCubeVBO);

    glBindBuffer(GL_ARRAY_BUFFER, lightCubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(lightCubeVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 3. Configure global OpenGL state
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 4. Build and compile shaders
    ourShader = std::make_unique<Shader>("shader.vs", "shader.fs");
    lightCubeShader = std::make_unique<Shader>("light_cube.vs", "light_cube.fs");
    roomShader = std::make_unique<Shader>("room_shader.vs", "room_shader.fs");
    bulletShader = std::make_unique<Shader>("bullet_shader.vs", "bullet_shader.fs");
    textShader = std::make_unique<Shader>("text_shader.vs", "text_shader.fs");
    enemyShader = std::make_unique<Shader>("enemy_shader.vs", "enemy_shader.fs");
    cannonShader = std::make_unique<Shader>("cannon_shader.vs", "cannon_shader.fs");
    radarShader = std::make_unique<Shader>("radarShader.vs", "radarShader.fs");
    gemShader = std::make_unique<Shader>("gem_shader.vs", "gem_shader.fs");
    puddleShader = std::make_unique<Shader>("puddle_shader.vs", "puddle_shader.fs");
    portalShader = std::make_unique<Shader>("porta_shader.vs", "porta_shader.fs");
    menuShader = std::make_unique<Shader>("menuShader.vs", "menuShaderF.fs");

    // 5. Load 3D models
    ourModel = std::make_unique<Model>("resources/robot/robot.obj");
    room = std::make_unique<Model>("resources/STANZA/stanza.obj");
    bulletModel = std::make_unique<Model>("resources/bullet/bullet2.obj");
    enemyModel = std::make_unique<Model>("resources/enemy/enemy.obj");
    bossModel = std::make_unique<Model>("resources/enemy/enemy.obj");
    cannonModel = std::make_unique<Model>("resources/cannone/cannone.obj");
    gemModel = std::make_unique<Model>("resources/gem/GEMMA.obj");
    puddleModel = std::make_unique<Model>("resources/Puddle/PUDDLE2.obj");
    portalModel = std::make_unique<Model>("resources/portale/PORTA.obj");

    // 6. Setup Menu quad
    float menuVertices[] = {
         1.0f,  1.0f,  1.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
        -1.0f,  1.0f,  0.0f, 1.0f 
    };
    unsigned int menuIndices[] = { 0, 1, 3, 1, 2, 3 };

    glGenVertexArrays(1, &menuVAO);
    glGenBuffers(1, &menuVBO);
    glGenBuffers(1, &menuEBO);

    glBindVertexArray(menuVAO);
    glBindBuffer(GL_ARRAY_BUFFER, menuVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(menuVertices), menuVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, menuEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(menuIndices), menuIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 7. Load menu texture
    glGenTextures(1, &menuTexture);
    glBindTexture(GL_TEXTURE_2D, menuTexture);
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("resources/images/droid_attack_menu.png", &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = GL_RGB;
        if (nrChannels == 4) format = GL_RGBA;
        else if (nrChannels == 1) format = GL_RED;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load menu texture" << std::endl;
    }
    stbi_image_free(data);

    // 8. Load Game Over texture
    glGenTextures(1, &gameOverMenuTexture);
    glBindTexture(GL_TEXTURE_2D, gameOverMenuTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int widthGO, heightGO, nrChannelsGO;
    unsigned char* dataGO = stbi_load("resources/images/game_over_menu.png", &widthGO, &heightGO, &nrChannelsGO, 0);
    if (dataGO) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthGO, heightGO, 0, GL_RGB, GL_UNSIGNED_BYTE, dataGO);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load game over menu texture" << std::endl;
    }
    stbi_image_free(dataGO);

    // 9. Compute room geometry & exits
    minRoom = getRoomMin(*room);
    maxRoom = getRoomMax(*room);
    roomSize = (maxRoom - minRoom) / glm::vec3(2.0f, 2.0f, 2.0f);
    exitRoom1R = glm::vec3(-roomSize.x + 2.0f, 0.0f, 0.0f);
    exitRoom2UP = glm::vec3(0.0f, 0.0f, roomSize.z - 2.0f);

    // 10. Bullets pool initialization
    bullets.clear();
    for (int i = 0; i < 4; i++) {
        std::string bulletname = "bullet" + std::to_string(i);
        Bullet b = { position, dir, false, bulletname };
        bullets.push_back(b);
    }

    Enemybullets.clear();
    for (int i = 0; i < 6; i++) {
        std::string bulletname = "bullet" + std::to_string(i);
        Bullet b = { position, dir, false, bulletname };
        Enemybullets.push_back(b);
    }

    return true;
}

void GameResources::Cleanup() {
    if (face) FT_Done_Face(face);
    if (ft) FT_Done_FreeType(ft);

    if (menuVAO) glDeleteVertexArrays(1, &menuVAO);
    if (menuVBO) glDeleteBuffers(1, &menuVBO);
    if (menuEBO) glDeleteBuffers(1, &menuEBO);
    if (menuTexture) glDeleteTextures(1, &menuTexture);
    if (gameOverMenuTexture) glDeleteTextures(1, &gameOverMenuTexture);

    if (lightCubeVAO) glDeleteVertexArrays(1, &lightCubeVAO);
    if (lightCubeVBO) glDeleteBuffers(1, &lightCubeVBO);
}
