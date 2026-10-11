#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Process continuous keyboard input
void processInput(GLFWwindow* window);

// Window resize callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Mouse movement callback
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

// Mouse scroll wheel callback
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// Mouse button press callback (menu interactions, shooting)
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
