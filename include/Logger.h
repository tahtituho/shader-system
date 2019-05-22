#ifndef LOGGER_H
#define LOGGER_H
#include <string>
#include <fstream>

namespace DemoSystem {
    class Logger {
        public:
            Logger();    
            ~Logger();

            enum LOG_LEVEL {
                INFO,
                ERR
            };
            void write(LOG_LEVEL level, std::string text);

        private:
            std::ofstream stream;
    };
}
#endif