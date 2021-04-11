#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "Graphics.h"
#include "Music.h"
#include "Logger.h"
#include "Synchronizer.h"
#include "Helpers.h"
#include "Camera.h"

namespace DemoSystem
{
    class InputDevices
    {
    public:
        InputDevices();
        ~InputDevices();
        void initialize(Graphics *graphics, Music *music, Logger *logger, Synchronizer *synchronizer, Camera *camera, bool release);
        void handleKeyboard(GLFWwindow *window, int key, int scancode, int action, int mods);
        void handleMouseMove(GLFWwindow *window, double x, double y);
        void handleMouseButtons(GLFWwindow *window, int button, int action, int mods);

    private:
        const float MOVEMENT_SPEED = 0.5;
        const float ROTATION_SPEED = 0.1;
        bool mouseButtonPressed;
        double previousMouseX;
        double previousMouseY;
        bool release;
        Graphics *graphics;
        Music *music;
        Synchronizer *synchronizer;
        Camera *camera;
        Logger *logger;
    };

} // namespace DemoSystem

#endif