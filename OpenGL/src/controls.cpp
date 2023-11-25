#include "controls.h"
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <utility>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <iostream>


float InputController::rotation = 0.0f;
float InputController::x_cam_pos = 0.0f;
float InputController::z_cam_pos = 0.0f;
float InputController::x_cam_angle = 0.0f;
float InputController::y_cam_angle = 0.0f;
float InputController::z_cam_angle = 0.0f;
unsigned int InputController::window_height = 0;
unsigned int InputController::window_width = 0;
float InputController::sensitivity = 1.0f;
float InputController::camera_speed = 0.03;


InputController& InputController::get_instance(unsigned int windows_height, unsigned int windows_width, float sensitivity)
{
    InputController::window_height = windows_height;
    InputController::window_width = windows_width;
    InputController::sensitivity = sensitivity;
    static InputController instance;
    return instance;
}

void InputController::cursor_position_callback(
    GLFWwindow* window, 
    double xpos, 
    double ypos)
{
    // Normalized change in cursos position since last action 
    float delta_x = ((window_width  / 2.0f - xpos) /window_width)  * sensitivity;
    float delta_y = ((window_height / 2.0f - ypos) /window_height) * sensitivity;

    // Change camera angle
    x_cam_angle += delta_x;
    y_cam_angle += delta_y;

    // Loop x angle 0-360
    if (x_cam_angle > glm::pi<float>() * 2.0f) {
        x_cam_angle -= glm::pi<float>() * 2.0f;
    }
    else if (x_cam_angle < 0.0f) {
        x_cam_angle += glm::pi<float>() * 2.0f;
    }

    // Limit y angle to [-pi/2, pi/2] 
    if (y_cam_angle > glm::pi<float>() / 2.0f) {
        y_cam_angle = glm::pi<float>() / 2.0f - 0.001f;
    }
    else if (y_cam_angle < -glm::pi<float>() / 2.0f) {
        y_cam_angle = -glm::pi<float>() / 2.0f + 0.001f;
    }

    // Move the cursos to the middle
    glfwSetCursorPos(window, window_width / 2.0f, window_height / 2.0f);
}

void InputController::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Move forward
    if (key == GLFW_KEY_W && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        z_cam_pos += cos(x_cam_angle) * camera_speed;
        x_cam_pos += sin(x_cam_angle) * camera_speed;
    }

    if (key == GLFW_KEY_A && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        z_cam_pos -= sin(x_cam_angle) * camera_speed;
        x_cam_pos += cos(x_cam_angle) * camera_speed;
    }
    if (key == GLFW_KEY_S && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        z_cam_pos -= cos(x_cam_angle) * camera_speed;
        x_cam_pos -= sin(x_cam_angle) * camera_speed;
    }

    if (key == GLFW_KEY_D && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        z_cam_pos += sin(x_cam_angle) * camera_speed;
        x_cam_pos -= cos(x_cam_angle) * camera_speed;
    }
}

glm::vec3 InputController::get_cam_pos()
{
    glm::vec3 cam_pos(x_cam_pos, 0.0f, z_cam_pos);
    return cam_pos;
}

glm::vec3 InputController::get_cam_dir()
{
    glm::vec3 cam_direction(cos(y_cam_angle) * sin(x_cam_angle) + x_cam_pos,
                            sin(y_cam_angle),
                            cos(y_cam_angle) * cos(x_cam_angle) + z_cam_pos);
    return cam_direction;
}