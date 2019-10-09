#include "Helpers.h"

std::string DemoSystem::Helpers::readFile(std::string file) {
    std::ifstream ifs(file);
    return std::string((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
}