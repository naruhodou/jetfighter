#include "main.h"
#include "cuboid.h"
using namespace std;
#ifndef PARACHUTE_H
#define PARACHUTE_H
#define GLM_ENABLE_EXPERIMENTAL

class Parachute {
public:
    Parachute() {}
    Parachute(glm::vec3 pos);
    glm::vec3 position;
    bool isdraw;
    float rotation;
    Cuboid lower, upper;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick(int sn);
};

#endif
