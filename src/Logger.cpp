#include "Logger.h"

DemoSystem::Logger::Logger()
{
}

DemoSystem::Logger::~Logger()
{
}

void DemoSystem::Logger::initialize(int size, float x, float y, bool e)
{
    this->size = size;
    this->x = x;
    this->y = y;
    this->enabled = e;

    gltInit();
}

void DemoSystem::Logger::write(DemoSystem::Logger::LOG_LEVEL level, std::string text)
{
    if (text.length() == 0)
    {
        return;
    }
    std::ostringstream stream;
    switch (level)
    {
    case INFO:
        stream << "[INFO] ";
        break;
    case ERR:
        stream << "[ERROR] ";
        break;
    }
    stream << text;

    if (this->buffer.size() >= this->size)
    {
        gltDeleteText(this->buffer.back());
        this->buffer.pop_back();
    }
    GLTtext *t = gltCreateText();
    gltSetText(t, stream.str().c_str());
    this->buffer.push_front(t);
}

void DemoSystem::Logger::render()
{
    if (this->enabled)
    {
        gltBeginDraw();
        gltColor(1.0f, 1.0f, 1.0f, 1.0f);

        float runningY = y - 15.0;
        for (GLTtext *text : this->buffer)
        {
            gltDrawText2D(text, this->x, runningY, 1.0);
            runningY -= 15.0;
        }
        gltEndDraw();
    }
}

bool DemoSystem::Logger::isEnabled()
{
    return this->enabled;
}

void DemoSystem::Logger::toggleEnable()
{
    this->enabled = !this->enabled;
}

void DemoSystem::Logger::cleanUp()
{
    for (GLTtext *text : this->buffer)
    {
        gltDeleteText(text);
    }
    gltTerminate();
}
