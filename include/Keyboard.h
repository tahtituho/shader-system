#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "Graphics.h"
#include "Music.h"
#include "Logger.h"
#include "Synchronizer.h"
#include "Helpers.h"

namespace DemoSystem
{
    class Keyboard
    {
    public:
        Keyboard();
        ~Keyboard();
        void initialize(Graphics graphics, Music music, Logger logger, bool release);
        void handleKeyboard(GLFWwindow *window, int key, int scancode, int action, int mods);

    private:
        bool release;
        Graphics graphics;
        Music music;
        Logger logger;
    };

} // namespace DemoSystem

#endif