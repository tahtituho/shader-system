#ifndef RESOURCE_H
#define RESOURCE_H
#include <string>

namespace DemoSystem {
    class Resource {
        public:
            Resource();
            virtual ~Resource() = default;
            std::string name;
    };
}
#endif