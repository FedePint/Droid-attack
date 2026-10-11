#include "RenderUtils.h"
#include "Globals.h"

void RenderPorta(glm::vec3 pos, glm::mat4 view, glm::mat4 projection, Shader& portaShader, Model& portal, glm::vec3 roomSize)
{
    float angle = 0.0f;
    glm::vec3 ExitRoom1R = glm::vec3(-roomSize.x + 2.0f, 0.0f, 0.0f);
    glm::vec3 ExitRoom2UP = glm::vec3(0.0f, 0.0f, roomSize.z - 2.0f);

    if (pos == -ExitRoom1R)
    {
        angle = 180.0f;
    }
    else if (pos == -ExitRoom2UP)
    {
        angle = 90.0f;
    }
    else if (pos == ExitRoom2UP)
    {
        angle = -90.0f;
    }
    else if (pos == ExitRoom1R)
    {
        angle = -180.0f;
    }
    pos = pos + glm::vec3(0.0f, 3.0f, 0.0f);
    portaShader.use();
    glm::mat4 porta = glm::mat4(1.0f);
    porta = glm::translate(porta, pos);
    porta = glm::scale(porta, glm::vec3(1.0f, 1.0f, 1.0f));
    porta = glm::rotate(porta, glm::radians(angle), glm::vec3(0, 1, 0));
    portaShader.setMat4("model", porta);
    portaShader.setMat4("view", view);
    portaShader.setMat4("projection", projection);
    portaShader.setVec3("light.position", lightPos);
    portaShader.setVec3("viewPos", camera.Position);
    portaShader.setVec3("light.ambient", 0.4f, 0.4f, 0.4f);
    portaShader.setVec3("light.diffuse", 0.4f, 0.4f, 0.4f);
    portaShader.setVec3("light.specular", 0.5f, 0.5f, 0.5f);

    portaShader.setFloat("material.shininess", 14.0f);
    portal.Draw(portaShader);
}

void RenderText(Shader& textShader, std::string text, float x, float y, float scale, glm::vec3 color)
{
    unsigned int VAO_T, VBO_T;
    glGenVertexArrays(1, &VAO_T);
    glGenBuffers(1, &VBO_T);
    glBindVertexArray(VAO_T);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_T);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    textShader.use();
    glUniform3f(glGetUniformLocation(textShader.ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO_T);

    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        float textVertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_T);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(textVertices), textVertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += (ch.Advance >> 6) * scale;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDeleteBuffers(1, &VBO_T);
    glDeleteVertexArrays(1, &VAO_T);
}
