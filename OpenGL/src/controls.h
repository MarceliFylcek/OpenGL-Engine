#pragma once
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <utility>
#include "utils.h"


class InputController
{
public:
    static InputController& get_instance(unsigned int windows_height, unsigned int windows_width,
                                        float sensitivity);
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static glm::vec3 get_cam_pos();
    static glm::vec3 get_cam_dir();
private:
    static cam_position cam_pos;
    static float rotation;
    static float x_cam_pos;
    static float z_cam_pos;
    static float x_cam_angle;
    static float y_cam_angle;
    static float z_cam_angle;
    static float camera_speed;
    static unsigned int window_height;
    static unsigned int window_width;
    static float sensitivity;
    InputController() = default;
};