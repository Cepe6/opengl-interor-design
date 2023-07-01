#include <iostream>
#include <vector>
#include "external/glfw-3.1.2/deps/glad/glad.h"
#include "headers/Camera.h"
#include "headers/Light.h"
#include "headers/ShaderManager.h"
#include "headers/PointLightManager.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;


float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

std::vector<Object*> room_objects = {};
std::vector<Light*> light_objects = {};

GLFWwindow* window;

bool lightStatus = false;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

int init();
void renderLoop();
void loadShaders();
void loadObjects();
void processInput();
void calculateDeltaTime();

int main() {
    if (init() == -1) {
        return -1;
    }

    loadShaders();
    loadObjects();
    renderLoop();

    glfwTerminate();
    return 0;
}

int init() {
    //new Camera();

    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Room", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetCursorPosCallback(window, mouse_callback); //
    glfwSetScrollCallback(window, scroll_callback);//
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    return 0;
}

void renderLoop() {
    while (!glfwWindowShouldClose(window))
    {
        calculateDeltaTime();

        processInput();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        ShaderManager::getShaderByName("texture")->setMat4("projection", projection);

        glm::mat4 view = camera.GetViewMatrix();
        ShaderManager::getShaderByName("texture")->setMat4("view", view);

        for (Object* gameObject : room_objects) {
            gameObject->draw();
        }

        for (Light* gameObject : light_objects) {
            gameObject->draw();
        }

        glfwSwapBuffers(window); // will swap the color buffer (a large 2D buffer that contains color values for each pixel in GLFW's window)
        glfwPollEvents(); // checks if any events are triggered (like keyboard input or mouse movement events)
    }
}

void processInput() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        PointLight* light = PointLightManager::getPointLights()[1];
        light->on = !light->on;
    }
}

void loadObjects() {
    Object* obj = new Object("cube1", glm::vec3(1.2f, 1.2f, 1.2f), glm::vec3(0.0f, 0.1f, 0.0f),
                             0.0f, glm::vec3(0.0f) + glm::vec3(0.0f, -1.8f, 0.0f), "./container-space.jpg");

    Object* obj2 = new Object("cube2", glm::vec3(1.2f, 1.2f, 1.2f), glm::vec3(0.0f, 0.1f, 0.0f),
                              0.0f, glm::vec3(0.0f) + glm::vec3(1.4f, -1.8f, 0.0f), "./container-space.jpg");

    Object* obj3 = new Object("cube3", glm::vec3(1.2f, 1.2f, 1.2f), glm::vec3(0.0f, 0.1f, 0.0f),
                              0.0f, glm::vec3(0.0f) + glm::vec3(1.0f, -0.6f, 0.0f), "./container-space.jpg");

    Object* panel = new Object("panel", glm::vec3(1.2f, 2.2f, 3.2f), glm::vec3(0.0f, 0.1f, 0.0f),
                               0.0f, glm::vec3(0.0f) + glm::vec3(7.0f, -1.4f, 0.0f), "./panel.jpg");

    Object* screen = new Object("screen", glm::vec3(0.01f, 3.2f, 1.6f), glm::vec3(90.0f, 0.1f, 0.0f),
                                90.0f, glm::vec3(0.0f) + glm::vec3(6.8f, 0.5f, 0.0f), "./screen.jpg");

    Object* portret = new Object("portret", glm::vec3(1.4f, 1.4f, 0.1f), glm::vec3(0.0f, 0.05f, 0.0f),
                                 0.0f, glm::vec3(0.0f) + glm::vec3(-3.0f, 0.6f, -7.15f), "./azis.jpg");

    Object* floor = new Object("floor", glm::vec3(15.0f, 0.1f, 15.0f),
                               glm::vec3(0.0f, 0.1f, 0.0f), 0.0f, glm::vec3(0.0f) + glm::vec3(0.0f, -2.5f, 0.0f), "./floor.jpg");

    Object* wall1 = new Object("wall1", glm::vec3(0.75f, 15.0f, 7.0f),
                               glm::vec3(90.0f, 0.1f, 0.0f), 90.0f, glm::vec3(0.0f) + glm::vec3(-7.5f, 0.0f, 0.0f), "./wall.jpg");

    Object* wall2 = new Object("wall2", glm::vec3(15.0f, 7.0f, 0.75f),
                               glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, glm::vec3(0.0f) + glm::vec3(0.0f, 0.0f, -7.5f), "./wall.jpg");

    Object* wall3 = new Object("wall3", glm::vec3(15.0f, 7.0f, 0.75f),
                               glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, glm::vec3(0.0f) + glm::vec3(0.0f, 0.0f, 7.5f), "./wall.jpg");

    Object* wall4 = new Object("wall4", glm::vec3(0.75f, 15.0f, 7.0f),
                               glm::vec3(90.0f, 0.1f, 0.0f), 90.0f, glm::vec3(0.0f) + glm::vec3(7.5f, 0.0f, 0.0f), "./wall.jpg");

    Object* ceiling = new Object("ceiling", glm::vec3(15.0f, 0.1f, 15.0f),
                                 glm::vec3(0.0f, 0.1f, 0.0f), 0.0f, glm::vec3(0.0f) + glm::vec3(0.0f, 2.5f, 0.0f), "./ceiling.jpg");

    room_objects.push_back(obj);
    room_objects.push_back(obj2);
    room_objects.push_back(obj3);

    room_objects.push_back(portret);
    room_objects.push_back(panel);
    room_objects.push_back(screen);

    room_objects.push_back(floor);
    room_objects.push_back(wall1);
    room_objects.push_back(wall2);
    room_objects.push_back(wall3);
    room_objects.push_back(wall4);
    room_objects.push_back(ceiling);

    Light* lamp = new Light("lamp", glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.0f, 0.1f, 0.0f),
                            0.0f, glm::vec3(0.0f) + glm::vec3(-5.0f, 0.8f, 0.0f), "./ceiling.jpg");

    Light* lamp2 = new Light("lamp2", glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.0f, 0.1f, 0.0f),
                             0.0f, glm::vec3(0.0f) + glm::vec3(5.0f, 0.8f, 0.0f), "./ceiling.jpg");

    light_objects.push_back(lamp);
    light_objects.push_back(lamp2);
}

void calculateDeltaTime()
{
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

void loadShaders()
{
    ShaderManager::addShader(new Shader("texture", "./texture_shader.vs", "./texture_shader.fs"));
    ShaderManager::addShader(new Shader("light", "./texture_lightsource.vs", "./texture_lightsource.fs"));
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
