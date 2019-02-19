#include "main.h"
#include "cuboid.h"
using namespace std;
#ifndef CANON_H
#define CANON_H
#define GLM_ENABLE_EXPERIMENTAL

class Canon {
public:
    Canon() {}
    Canon(glm::vec3 pos, float rotation);
    glm::vec3 position;
    bool isdraw;
    float rotation;
    Cuboid lower, upper;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick(int sn);
};

#endif
