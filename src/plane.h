#include "main.h"
using namespace std;
#ifndef PLANE_H
#define PLANE_H
#define GLM_ENABLE_EXPERIMENTAL

class Plane {
public:
    Plane() {}
    Plane(glm::vec3 pos, glm::vec3 dim, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick(int sn);
private:
    VAO *object;
};

#endif
