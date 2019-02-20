#include "main.h"
using namespace std;
#ifndef COMPASS_H
#define COMPASS_H
#define GLM_ENABLE_EXPERIMENTAL

class Compass {
public:
    Compass() {}
    Compass(glm::vec3 pos, float side);
    glm::vec3 position;
    bool isdraw;
    float rotation;
    void draw(glm::mat4 VP);
private:
    VAO *object1, *object2;
};

#endif
