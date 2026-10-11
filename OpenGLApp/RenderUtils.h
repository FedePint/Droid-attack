#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include "shader.h"
#include "model.h"

// Render text onto screen using FreeType glyph textures
void RenderText(Shader& textShader, std::string text, float x, float y, float scale, glm::vec3 color);

// Render portal model with lighting and transformation
void RenderPorta(glm::vec3 pos, glm::mat4 view, glm::mat4 projection, Shader& portaShader, Model& portal, glm::vec3 roomSize);
