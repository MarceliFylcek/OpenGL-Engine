#pragma once
#include <gl/glew.h>
#include <GLFW/glfw3.h>


class InputController
{
public:
    static InputController& get_instance();
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static float get_rotation();
private:
    static float rotation;
    InputController() = default;
};