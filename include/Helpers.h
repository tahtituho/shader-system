#ifndef HELPERS_H
#define HELPERS_H
#include <string>
#include <fstream>

namespace DemoSystem {
    class Helpers {
        public:
            static std::string readFile(std::string file);
    };
}
#endif