#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <map>

// Struct representing a projectile
struct Bullet {
    glm::vec3 position;
    glm::vec3 direction;
    bool active;
    std::string name;
};

// Struct representing a FreeType character glyph
struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2 Size;        // Size of glyph
    glm::ivec2 Bearing;     // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};
