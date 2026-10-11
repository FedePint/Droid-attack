#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "GameResources.h"
#include "Globals.h"
#include "HUDRenderer.h"
#include "Input.h"
#include "MenuRenderer.h"
#include "RadarRenderer.h"
#include "SceneRenderer.h"

int main() {
  // Audio engine startup check
  if (!engine) {
    return 0; // Error starting up the engine
  }
  engine->play2D("resources/media/getout.wav", true);
  engine->setSoundVolume(1.0f);

  // Initialize and configure GLFW
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWmonitor *primary = glfwGetPrimaryMonitor();
  const GLFWvidmode *mode = glfwGetVideoMode(primary);
  SCR_WIDTH = mode->width;
  SCR_HEIGHT = mode->height;

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // Create GLFW window fullscreen
  GLFWwindow *window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "NASA_PUNK", primary, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetMouseButtonCallback(window, mouse_button_callback);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

  // Initialize GLAD function pointers
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // Load all shaders, models, textures, and fonts
  GameResources resources;
  if (!resources.Init()) {
    std::cout << "Failed to initialize game resources" << std::endl;
    glfwTerminate();
    return -1;
  }

  int roomActive = 0;
  int roomPrev = 0;

  // Main Game Loop
  while (!glfwWindowShouldClose(window)) {
    // Per-frame timing logic
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // Process continuous keyboard input
    processInput(window);

    // State 0: Main Menu
    if (currentState == 0) {
      RenderMainMenu(resources, window);
      continue;
    }
    // State -1: Game Over
    else if (currentState == -1) {
      RenderGameOver(resources, window);
      continue;
    }

    // State 1: Active 3D Gameplay
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
                                            static_cast<float>(SCR_WIDTH) /
                                                static_cast<float>(SCR_HEIGHT),
                                            0.1f, 150.0f);
    glm::mat4 view = camera.GetViewMatrix();

    RenderScene(resources, roomActive, roomPrev, view, projection);
    RenderHUD(resources);
    RenderRadar(resources, roomActive);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Cleanup resources
  resources.Cleanup();
  if (engine) {
    engine->drop();
  }
  glfwTerminate();

  return 0;
}
