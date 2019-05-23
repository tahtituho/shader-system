#include "Logger.h"

DemoSystem::Logger::Logger() {
    this->stream = std::ofstream("log.txt", std::ios_base::out | std::ios_base::trunc);
}

DemoSystem::Logger::~Logger() {
    this->stream.close();
}

void DemoSystem::Logger::write(DemoSystem::Logger::LOG_LEVEL level, std::string text) {
    if(this->stream.is_open()) {
        switch(level) {
            case INFO:
                this->stream << "[INFO] ";
            break;
            case ERR:
                this->stream << "[ERROR] ";
            break;
        }
        this->stream << text << std::endl;
    }
}
