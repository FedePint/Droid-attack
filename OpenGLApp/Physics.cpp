#include "Physics.h"
#include <cfloat>
#include <algorithm>

bool hitbox(glm::vec3 centro1, double r1, glm::vec3 centro2, double r2)
{
    double dx = centro2.x - centro1.x;
    double dz = centro2.z - centro1.z;

    double distanzaQuad = dx * dx + dz * dz;
    double sommaRaggi = r1 + r2;

    return distanzaQuad <= (sommaRaggi * sommaRaggi);
}

glm::vec3 rotateY(const glm::vec3& v, float degrees) {
    float radians = glm::radians(degrees);
    glm::mat4 rot = glm::rotate(glm::mat4(1.0f), radians, glm::vec3(0, 1, 0));
    glm::vec4 res = rot * glm::vec4(v, 0.0f);
    return glm::normalize(glm::vec3(res));
}

glm::vec3 getRoomMin(const Model& room) {
    glm::vec3 min(FLT_MAX);
    for (const auto& mesh : room.meshes) {
        for (const auto& vertex : mesh.vertices) {
            min.x = std::min(min.x, vertex.Position.x);
            min.y = std::min(min.y, vertex.Position.y);
            min.z = std::min(min.z, vertex.Position.z);
        }
    }
    return min;
}

glm::vec3 getRoomMax(const Model& room) {
    glm::vec3 max(-FLT_MAX);
    for (const auto& mesh : room.meshes) {
        for (const auto& vertex : mesh.vertices) {
            max.x = std::max(max.x, vertex.Position.x);
            max.y = std::max(max.y, vertex.Position.y);
            max.z = std::max(max.z, vertex.Position.z);
        }
    }
    return max;
}
