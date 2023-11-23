#include "controls.h"
#include <gl/glew.h>
#include <GLFW/glfw3.h>


InputController& InputController::get_instance()
{
    static InputController instance;
    return instance;
}

void InputController::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    glfwSetCursorPos(window, 0, 0);

}

void InputController::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_X && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        rotation -= 0.1f;
    }

    if (key == GLFW_KEY_Z && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        rotation += 0.1f;
    }
}

float InputController::get_rotation()
{
    return rotation;
}

