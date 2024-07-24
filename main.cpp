#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "camera.h"
#include "shader.h"
#include "model.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffest);

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

float lastX = (float)SCR_WIDTH / 2;
float lastY = (float)SCR_HEIGHT / 2;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// light pos
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

bool mouseEnabled = true;

int main() {
  // camera.setFPSCam();
  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // glfw window creation
  // --------------------
  GLFWwindow *window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);
  // capture mouse
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // stbi_set_flip_vertically_on_load(true);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);
  glEnable(GL_CULL_FACE);

  // setup dearimgui
    // -----------------------------
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // imgui backend
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

  // build and compile our shader program
  // ------------------------------------
  Shader modelShader("../shaders/vertShader.vs", "../shaders/fragShader.fs");
  // shader variables
    glm::vec3 dirLightDirection(-0.2f, -1.0f, -0.3f);
    glm::vec3 dirLightAmbient(0.5f, 0.5f, 0.5f);
    glm::vec3 dirLightDiffuse(0.8f, 0.8f, 0.8f);
    glm::vec3 dirLightSpecular(0.5f, 0.5f, 0.5f);

    glm::vec3 pointLightPosition(0.0f, 2.0f, 2.0f);
    glm::vec3 pointLightAmbient(0.05f, 0.05f, 0.05f);
    glm::vec3 pointLightDiffuse(01.8f, 01.8f, 010.8f);
    glm::vec3 pointLightSpecular(1.0f, 1.0f, 1.0f);
    float pointLightConstant = 1.0f;
    float pointLightLinear = 0.09f;
    float pointLightQuadratic = 0.032f;

    float materialShininess = 64.0f;

      // load models
      // -------------------
      // change model here
      Model ourModel("../assets/models/pistol/pistol.obj");
      Model bModel("../assets/models/sword/sword.obj");

      // draw wireframe
      // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

      // render loop
      // -----------
      while (!glfwWindowShouldClose(window))
      {
        // delta time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.10f, 0.10f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glCullFace(GL_BACK);
        // be sure to activate shader when setting uniforms/drawing objects
        modelShader.use();

        // view/projection transformations
        glm::mat4 projection =
            glm::perspective(glm::radians(camera.Zoom),
                             (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        modelShader.setMat4("projection", projection);
        modelShader.setMat4("view", view);

        // light properties
        // directional light
        modelShader.setVec3("dirLight.direction", dirLightDirection);
        modelShader.setVec3("dirLight.ambient", dirLightAmbient);
        modelShader.setVec3("dirLight.diffuse", dirLightDiffuse);
        modelShader.setVec3("dirLight.specular", dirLightSpecular);
        // point light 1
        modelShader.setVec3("pointLight.position", pointLightPosition);
        modelShader.setVec3("pointLight.ambient", pointLightAmbient);
        modelShader.setVec3("pointLight.diffuse", pointLightDiffuse);
        modelShader.setVec3("pointLight.specular", pointLightSpecular);
        modelShader.setFloat("pointLight.constant", pointLightConstant);
        modelShader.setFloat("pointLight.linear", pointLightLinear);
        modelShader.setFloat("pointLight.quadratic", pointLightQuadratic);
        // material properties
        modelShader.setFloat("material.shininess", materialShininess);

        // render loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        modelShader.setMat4("model", model);
        ourModel.Draw(modelShader);
        // render loaded model
        glm::mat4 modelb = glm::mat4(1.0f);
        modelb = glm::translate(modelb, glm::vec3(0.0f, 0.0f, -5.0f));
        modelb = glm::scale(modelb, glm::vec3(1.0f, 1.0f, 1.0f));
        modelShader.setMat4("model", modelb);
        bModel.Draw(modelShader);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved
        // etc.)
        // -------------------------------------------------------------------------------
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // set window size and pos
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowSize(ImVec2((float)(SCR_WIDTH * 0.45), (float)(SCR_WIDTH * 0.45)));
        { // IMGUI Scene Window
          ImGui::Begin("Scene Settings", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
          ImGui::Text("Directional Light Settings");
          ImGui::SliderFloat3("Direction", (float *)&dirLightDirection, -10, 10, "%.3f");
          ImGui::SliderFloat3("Directional Ambient", (float *)&dirLightAmbient, 0, 10, "%.3f");
          ImGui::SliderFloat3("Directional Diffuse", (float *)&dirLightDiffuse, 0, 10, "%.3f");
          ImGui::SliderFloat3("Directional Specular", (float *)&dirLightSpecular, 0, 10, "%.3f");
          ImGui::Text("Point Light Settings");
          ImGui::SliderFloat3("Point Position", (float *)&pointLightPosition, -10, 10, "%.3f");
          ImGui::SliderFloat3("Point Ambient", (float *)&pointLightAmbient, 0, 10, "%.3f");
          ImGui::SliderFloat3("Point Diffuse", (float *)&pointLightDiffuse, 0, 10, "%.3f");
          ImGui::SliderFloat3("Point Specular", (float *)&pointLightSpecular, 0, 10, "%.3f");
          ImGui::SliderFloat("Point Constant", (float *)&pointLightConstant, 0, 10, "%.3f");
          ImGui::SliderFloat("LPoint inear", (float *)&pointLightLinear, 0, 5, "%.3f");
          ImGui::SliderFloat("Point Quadratic", (float *)&pointLightQuadratic, 0, 5, "%.4f");
          ImGui::Text("Material Settings");
          ImGui::SliderFloat("Material Shininess", (float *)&materialShininess, 0, 100, "%.2f");
          if(ImGui::Button("Close Program"))
            glfwSetWindowShouldClose(window, true);
          ImGui::End();
        }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
  }
  // shutdown imgui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    mouseEnabled = false;
  }
  if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
    firstMouse = true;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    mouseEnabled = true;
    }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(RIGHT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    camera.ProcessKeyboard(UP, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    camera.ProcessKeyboard(DOWN, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
  if (mouseEnabled) {
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);
  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;
  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
  }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffest) {
  camera.ProcessMouseScroll(static_cast<float>(yoffest));
}