#ifndef VARIABLE_H
#define VARIABLE_H
#include <string>
#include <GL/gl.h>
#include <sync.h>
namespace DemoSystem
{
    class Variable
    {
    public:
        enum DataType
        {
            FLOAT1,
            FLOAT2,
            FLOAT3
        };
        struct Vector3
        {
            double x;
            double y;
            double z;
        };
        Vector3 value;
        Variable::DataType type;
        std::string name;
        GLint uniform;
    };

    class TrackVariable : public Variable
    {
    public:
        struct SyncTrack
        {
            const sync_track *x;
            const sync_track *y;
            const sync_track *z;
        };
        SyncTrack syncTrack;
    };

    class BasicVariable : public Variable
    {
    };
}
#endif