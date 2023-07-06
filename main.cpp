#include <iostream>
#include <vector>

#include "headers/Camera.h"
#include "headers/Light.h"
#include "headers/ShaderManager.h"
#include "headers/PointLightManager.h"

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float last_x = SCREEN_WIDTH / 2.0f;
float last_y = SCREEN_HEIGHT / 2.0f;
bool first_mouse = true;
bool space_pressed_last_frame = false;
bool window_light_keys_pressed_last_frame[4] = {false, false, false, false};

float delta_time = 0.0f; // Time between current frame and last frame
float last_frame = 0.0f;  // Time of last frame

std::vector<Object*> room_objects = {};
std::vector<Light*> light_objects = {};

GLFWwindow* window;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

int init();
void render_loop();
void load_shaders();
void load_objects();
void process_input();
void calculate_delta_time();

int main() {
    if (init() == -1) {
        return -1;
    }

    load_shaders();
    load_objects();
    render_loop();

    glfwTerminate();
    return 0;
}

int init() {
    //new Camera();

    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

    // glfw window creation
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Room", nullptr, nullptr);
    if (window == nullptr)
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
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2);
    glEnable(GL_DEPTH_TEST);

    return 0;
}

void render_loop() {
    while (!glfwWindowShouldClose(window))
    {
        calculate_delta_time();
        process_input();

        glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        ShaderManager::get_shader_by_name("texture")->setMat4("projection", projection);

        glm::mat4 view = camera.get_view_matrix();
        ShaderManager::get_shader_by_name("texture")->setMat4("view", view);

        for (Object* room_object : room_objects) {
            room_object->draw();
        }

        for (Light* light_object : light_objects) {
            light_object->draw();
        }

        glfwSwapBuffers(window); // will swap the color buffer (a large 2D buffer that contains color values for each pixel in GLFW's window)
        glfwPollEvents(); // checks if any events are triggered (like keyboard input or mouse movement events)
    }
}

void process_input() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.on_keyboard_input(FORWARD, delta_time, glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.on_keyboard_input(BACKWARD, delta_time, glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.on_keyboard_input(LEFT, delta_time, glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.on_keyboard_input(RIGHT, delta_time, glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);

    // Sunrise window color
    if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        window_light_keys_pressed_last_frame[0] = true;
    } else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_RELEASE && window_light_keys_pressed_last_frame[0]) {
        window_light_keys_pressed_last_frame[0] = false;
        PointLight* light = PointLightManager::get_point_light_by_name("window_light");
        light->ambient = glm::vec3(5.0f, 3.96f, 2.43f) * 0.5f;
    }
    // Midday window color
    if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        window_light_keys_pressed_last_frame[1] = true;
    } else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_RELEASE && window_light_keys_pressed_last_frame[1]) {
        window_light_keys_pressed_last_frame[1] = false;
        PointLight* light = PointLightManager::get_point_light_by_name("window_light");
        light->ambient = glm::vec3(5.0f, 5.0f, 2.19f) * 0.5f;
    }
    // Sunset window color
    if(glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        window_light_keys_pressed_last_frame[2] = true;
    } else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_RELEASE && window_light_keys_pressed_last_frame[2]) {
        window_light_keys_pressed_last_frame[2] = false;
        PointLight* light = PointLightManager::get_point_light_by_name("window_light");
        light->ambient = glm::vec3(4.9f, 4.19f, 3.23f) * 0.5f;
    }
    // Full-moon window color
    if(glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
        window_light_keys_pressed_last_frame[3] = true;
    } else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_RELEASE && window_light_keys_pressed_last_frame[3]) {
        window_light_keys_pressed_last_frame[3] = false;
        PointLight* light = PointLightManager::get_point_light_by_name("window_light");
        light->ambient = glm::vec3(1.21f, 1.49f, 2.31f) * 0.5f;
    }

    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        space_pressed_last_frame = true;
    } else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE && space_pressed_last_frame) {
        space_pressed_last_frame = false;
        PointLight* light = PointLightManager::get_point_light_by_name("screen_light");
        light->on = !light->on;
    }
}

void load_objects() {
    auto* red_chair = new Object("cube1",
                                 glm::vec3(1.2f, 1.2f, 1.2f),
                                 glm::vec3(0.0f, 0.1f, 0.0f),
                                 0.0f,
                                 glm::vec3(0.0f) + glm::vec3(-6.0f, -1.8f, -6.0f),
                                 "../resources/chair-red-leather.jpg");

    auto* brown_chair = new Object("cube2",
                                   glm::vec3(1.2f, 1.2f, 1.2f),
                                   glm::vec3(0.0f, 0.1f, 0.0f),
                                   0.0f,
                                   glm::vec3(0.0f) + glm::vec3(3.0f, -1.8f, 1.0f),
                                   "../resources/chair-brown-leather.jpg");

    auto* black_chair = new Object("cube3",
                                   glm::vec3(1.2f, 1.2f, 1.2f),
                                   glm::vec3(0.0f, 0.1f, 0.0f),
                                   0.0f,
                                   glm::vec3(0.0f) + glm::vec3(3.0f, -1.8f, -1.0f),
                                   "../resources/chair-black-leather.jpg");

    auto* screen = new Object("screen",
                              glm::vec3(0.01f, 3.2f, 1.6f),
                              glm::vec3(90.0f, 0.1f, 0.0f),
                              90.0f,
                              glm::vec3(0.0f) + glm::vec3(7.1f, 0.5f, 0.0f),
                              "../resources/screen.jpg");

    auto* wall_window = new Object("window",
                                   glm::vec3(2.0f, 2.0f, 0.1f),
                                   glm::vec3(0.0f, 0.05f, 0.0f),
                                   0.0f,
                                   glm::vec3(0.0f) + glm::vec3(-3.0f, 0.6f, -7.15f),
                                   "../resources/window.jpg");

    auto* floor = new Object("floor",
                             glm::vec3(15.0f, 0.1f, 15.0f),
                             glm::vec3(0.0f, 0.1f, 0.0f),
                             0.0f,
                             glm::vec3(0.0f) + glm::vec3(0.0f, -2.5f, 0.0f),
                             "../resources/floor.jpg");

    auto* wall1 = new Object("wall1",
                             glm::vec3(0.75f, 15.0f, 7.0f),
                             glm::vec3(90.0f, 0.1f, 0.0f),
                             90.0f,
                             glm::vec3(0.0f) + glm::vec3(-7.5f, 0.0f, 0.0f),
                             "../resources/wood-wall.jpg");

    auto* wall2 = new Object("wall2",
                             glm::vec3(15.0f, 7.0f, 0.75f),
                             glm::vec3(0.0f, 1.0f, 0.0f),
                             0.0f,
                             glm::vec3(0.0f) + glm::vec3(0.0f, 0.0f, -7.5f),
                             "../resources/wood-wall.jpg");

    auto* wall3 = new Object("wall3",
                             glm::vec3(15.0f, 7.0f, 0.75f),
                             glm::vec3(0.0f, 1.0f, 0.0f),
                             0.0f,
                             glm::vec3(0.0f) + glm::vec3(0.0f, 0.0f, 7.5f),
                             "../resources/wood-wall.jpg");

    auto* wall4 = new Object("wall4",
                             glm::vec3(0.75f, 15.0f, 7.0f),
                             glm::vec3(90.0f, 0.1f, 0.0f),
                             90.0f,
                             glm::vec3(0.0f) + glm::vec3(7.5f, 0.0f, 0.0f),
                             "../resources/wood-wall.jpg");

    auto* ceiling = new Object("ceiling",
                               glm::vec3(15.0f, 0.1f, 15.0f),
                               glm::vec3(0.0f, 0.1f, 0.0f),
                               0.0f,
                               glm::vec3(0.0f) + glm::vec3(0.0f, 2.5f, 0.0f),
                               "../resources/ceiling.jpg");

    room_objects.push_back(red_chair);
    room_objects.push_back(black_chair);
    room_objects.push_back(brown_chair);

    room_objects.push_back(wall_window);
    room_objects.push_back(screen);

    room_objects.push_back(floor);
    room_objects.push_back(wall1);
    room_objects.push_back(wall2);
    room_objects.push_back(wall3);
    room_objects.push_back(wall4);
    room_objects.push_back(ceiling);

    auto* screen_light = new Light("window_light",
                           glm::vec3(0.2f, 0.2f, 0.2f),
                           glm::vec3(0.0f, 0.1f, 0.0f),
                           glm::vec3(1.21f, 1.49f, 2.31f),
                           0.0f,
                           glm::vec3(0.0f) + glm::vec3(-3.0f, 0.5f, -7.5f));

    auto* window_light = new Light("screen_light",
                            glm::vec3(0.2f, 0.2f, 0.2f),
                            glm::vec3(0.0f, 0.1f, 0.0f),
                            glm::vec3(2.5f, 3.5f, 5.0f),
                            0.0f,
                            glm::vec3(0.0f) + glm::vec3(7.5f, 0.8f, 0.0f));

    light_objects.push_back(screen_light);
    light_objects.push_back(window_light);
}

void calculate_delta_time()
{
    auto currentFrame = (float) glfwGetTime();
    delta_time = currentFrame - last_frame;
    last_frame = currentFrame;
}

void load_shaders()
{
    ShaderManager::add_shader(new Shader("texture",
                                         "../shaders/texture_shader.vs",
                                         "../shaders/texture_shader.fs"));

    ShaderManager::add_shader(new Shader("light",
                                         "../shaders/texture_lightsource.vs",
                                         "../shaders/texture_lightsource.fs"));
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    auto x_pos = static_cast<float>(xposIn);
    auto y_pos = static_cast<float>(yposIn);

    if (first_mouse)
    {
        last_x = x_pos;
        last_y = y_pos;
        first_mouse = false;
    }

    float xoffset = x_pos - last_x;
    float yoffset = last_y - y_pos; // reversed since y-coordinates go from bottom to top

    last_x = x_pos;
    last_y = y_pos;

    camera.on_mouse_movement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.on_mouse_scroll(static_cast<float>(yoffset));
}